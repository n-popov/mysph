#include <iostream>
#include <numeric>
#include <vector>

#include "utils/vtk.h"
#include "utils/kernel.h"
#include "utils/particle.h"

auto max_time = 1e-2;
auto dt = 5e-4;

auto N = 10;
auto scale = 0.1;
auto h = 0.3;
auto nu = 1.1;
auto rho0 = 1000.;
auto k = 2.2e6;

int at(int N, int ix, int iy, int iz) {
    return ix * N * N + iy * N + iz;
}


int main() {
    std::vector<mysph::Particle<double>> particles0;
    for (auto ix = 0; ix < N; ix++) {
        for (auto iy = 0; iy < N; iy++) {
            for (auto iz = 0; iz < N; iz++) {
                particles0.push_back(mysph::Particle<double>({ix * scale, iy * scale, iz * scale}, {1., 0., 0.}, rho0, 1.));
            }
        }
    }

    std::vector<std::vector<mysph::Particle<double>>> particles_evolution = {particles0};
    particles_evolution.resize(max_time / dt);
    std::cout << std::size(particles_evolution);

    for (auto ti = 0; ti + 1 < std::size(particles_evolution); ti++) {
        auto& particles = particles_evolution[ti];
        auto& next_particles = particles_evolution[ti + 1];
        next_particles.resize(std::size(particles));
        
        for (auto i = 0; i < particles.size(); i++) {
            // Density
            particles[i].rho = 0.;
            for (auto j = 0; j < particles.size(); j++) {
                particles[i].rho += particles[j].m * mysph::kernel(particles[i].r - particles[j].r, h);
            }
            

            // Viscosity Force
            particles[i].Fv = {0., 0., 0.};

            for (auto j = 0; j < particles.size(); j++) {
                auto rij = particles[i].r - particles[j].r;
                auto den = particles[j].rho * mysph::abs(rij);
                if (den == 0) continue;
                particles[i].Fv = particles[i].Fv - (particles[i].v - particles[j].v) * (particles[j].m * 2 * mysph::abs(mysph::grad_kernel(rij, h)) / (den));
            }

            particles[i].Fv = particles[i].Fv * (particles[i].m * nu);

            // Pressure
            particles[i].p = k * (particles[i].rho - rho0);

            // Force
            particles[i].F = {0., 0., 0.};

            for (auto j = 0; j < particles.size(); j++) {
                if (particles[i].rho == 0 || particles[j].rho == 0) continue;

                auto rij = particles[i].r - particles[j].r;
                particles[i].F = particles[i].F - mysph::grad_kernel(rij, h) * (particles[j].m * (particles[i].p / std::pow(particles[i].rho, 2) + particles[j].p / std::pow(particles[j].rho, 2)));
            }

            // V splitted
            particles[i].vstar = particles[i].v + (particles[i].Fv) * (dt / particles[i].m);

            next_particles[i].v = particles[i].vstar + particles[i].F * (dt / particles[i].m);
            next_particles[i].r = particles[i].r + next_particles[i].v * dt; 

            next_particles[i].m = particles[i].m;
        }

        std::filesystem::create_directory("output");
        std::string filename = "output/mysph-" + std::to_string(ti) + ".vtp";

        write_particles_vtk(filename, particles);
    }
}