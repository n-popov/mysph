#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkXMLPolyDataWriter.h>

#include "particle.h"

using namespace mysph;

template <typename T>
void write_particles_vtk(const std::string& filename, const std::vector<Particle<T>>& particles) {
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();
    for (const auto& p : particles) {
        points->InsertNextPoint(p.r[0], p.r[1], p.r[2]);
    }

    auto vertices = vtkSmartPointer<vtkCellArray>::New();
    for (vtkIdType i = 0; i < particles.size(); ++i) {
        vertices->InsertNextCell(1, &i);
    }

    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetVerts(vertices);

    auto add_vector_field = [&](const char* name, const vec3<T> Particle<T>::* field) {
        auto array = vtkSmartPointer<vtkFloatArray>::New();
        array->SetName(name);
        array->SetNumberOfComponents(3);
        array->SetNumberOfTuples(particles.size());
        
        for (size_t i = 0; i < particles.size(); ++i) {
            const auto& val = particles[i].*field;
            array->SetTuple3(i, val[0], val[1], val[2]);
        }
        polyData->GetPointData()->AddArray(array);
    };

    auto add_scalar_field = [&](const char* name, const T Particle<T>::* field) {
        auto array = vtkSmartPointer<vtkFloatArray>::New();
        array->SetName(name);
        array->SetNumberOfComponents(1);
        array->SetNumberOfTuples(particles.size());
        
        for (size_t i = 0; i < particles.size(); ++i) {
            array->SetTuple1(i, particles[i].*field);
        }
        polyData->GetPointData()->AddArray(array);
    };

    add_vector_field("velocity", &Particle<T>::v);
    add_vector_field("vstar", &Particle<T>::vstar);
    add_vector_field("Fv", &Particle<T>::Fv);
    add_vector_field("F", &Particle<T>::F);
    add_scalar_field("density", &Particle<T>::rho);
    add_scalar_field("pressure", &Particle<T>::p);
    add_scalar_field("mass", &Particle<T>::m);

    auto writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(filename.c_str());
    writer->SetInputData(polyData);
    writer->Write();
}