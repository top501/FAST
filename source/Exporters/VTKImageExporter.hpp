#ifndef VTKIMAGEEXPORTER_HPP_
#define VTKIMAGEEXPORTER_HPP_

#include <vtkImageAlgorithm.h>
#include "ProcessObject.hpp"
#include "Image.hpp"

namespace fast {

class VTKImageExporter : public vtkImageAlgorithm, public ProcessObject {
    public:
        vtkTypeMacro(VTKImageExporter,vtkImageAlgorithm);
        static VTKImageExporter *New();
        void SetInput(Image::pointer image);
    private:
        Image::pointer mInput;
        VTKImageExporter();
        int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
        void execute() {};

};

} // end namespace fast



#endif /* VTKIMAGEEXPORTER_HPP_ */
