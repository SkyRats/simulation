 
#include "../include/simulation/getDropzonePositions.h"
#include <iostream>
#include <fstream>

static const double equatorial_radius = 6378197.0;
static const double flattening = 1.0/298.257223563;
static const double excentrity2 = 2*flattening - flattening*flattening;

static const double DEFAULT_REFERENCE_LATITUDE = 49.9;
static const double DEFAULT_REFERENCE_LONGITUDE = 8.9;
static const double DEFAULT_REFERENCE_HEADING   = 0.0;
static const double DEFAULT_REFERENCE_ALTITUDE  = 0.0;

namespace gazebo
{

    void getDropzonePositions::DropzonesCallback(ConstIntPtr &msg){
        gzmsg << "Dropzone pose:"<< "\n\tX = " << this->CP_V[0] - this->Iris_V[0]  << "\n\tY = " << this->CP_V[1] - this->Iris_V[1] << "\n\tZ = " << this->CP_V[2] - this->Iris_V[2] << std::endl;
    }
    // Pointer to the model
    physics::ModelPtr model;

    // Pointer to the pole link
    physics::LinkPtr link;
        
    // Pointer to the update event connection
    event::ConnectionPtr updateConnection;

    // Setup
    void getDropzonePositions::Setup(sdf::ElementPtr _sdf) 
    {
        reference_latitude_ = DEFAULT_REFERENCE_LATITUDE;
        reference_longitude_ = DEFAULT_REFERENCE_LONGITUDE;
        reference_heading_ = DEFAULT_REFERENCE_HEADING;
        reference_altitude_ = DEFAULT_REFERENCE_ALTITUDE;

        //_sdf->GetElement("latitude_deg")->GetValue()->Get(reference_latitude_);
        //_sdf->GetElement("longitude_deg")->GetValue()->Get(reference_longitude_);
        //_sdf->GetElement("heading_deg")->GetValue()->Get(reference_heading_);
        reference_latitude_ = 18.635216;
        reference_longitude_ = -98.522657;
        reference_heading_ = 0;

        reference_heading_ *= M_PI/180.0;

        double temp = 1.0 / (1.0 - excentrity2 * sin(reference_latitude_ * M_PI/180.0) * sin(reference_latitude_ * M_PI/180.0));
        double prime_vertical_radius = equatorial_radius * sqrt(temp);
        radius_north_ = prime_vertical_radius * (1 - excentrity2) * temp;
        radius_east_  = prime_vertical_radius * cos(reference_latitude_ * M_PI/180.0);
    }

    // Called on model creation
    void getDropzonePositions::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {

        //if (_sdf->HasElement("robotNamespace"))
        //    namespace_ = _sdf->GetElement("robotNamespace")->Get<std::string>();
        //else
        //    gzerr << "[a";
        //gzmsg << "AQUI1" << std::endl;
        node_handle_ = transport::NodePtr(new transport::Node());
        node_handle_->Init(namespace_);
        //gzmsg << "AQUI2" << std::endl;
        dropzone_sub = node_handle_->Subscribe("~/dropzones_pos", &getDropzonePositions::DropzonesCallback, this);
        
        Setup(_sdf);
        ignition::math::Pose3d dz_clean_pose = _parent->WorldPose();
        ignition::math::Vector3<double> dzc_p = dz_clean_pose.Pos();
        this->CP_V[0] = dzc_p.X();
        this->CP_V[1] = dzc_p.Y();
        this->CP_V[2] = dzc_p.Z();
        
        physics::WorldPtr puebla = _parent->GetWorld();
          physics::ModelPtr iris;
          int iris_status = 0;
        try{
        iris = puebla->ModelByName("spawn2"); iris_status = 1;} catch(std::runtime_error *e) {}
        //try{
        //if(iris_status == 0) iris = puebla->ModelByName("iris_0");} catch(std::runtime_error *e) {}
        ignition::math::Pose3d iris_pose;
        iris_pose = iris->WorldPose();
        
        ignition::math::Vector3<double> iris_p = iris_pose.Pos();
        
        this->Iris_V[0] = iris_p.X();
        this->Iris_V[1] = iris_p.Y();
        this->Iris_V[2] = iris_p.Z();
        
        //gzmsg << "Dropzone GPS:\n\t Latitude = " << reference_latitude_  + ( cos(reference_heading_) * CP_V[0] + sin(reference_heading_) * CP_V[1]) / radius_north_ * 180.0/M_PI << "\n\t Longitude = " << reference_longitude_ - (-sin(reference_heading_) * CP_V[0] + cos(reference_heading_) * CP_V[1]) / radius_east_  * 180.0/M_PI << "\n"<< std::endl;
        gzmsg << "Dropzone pose:"<< "\n\tX = " << this->CP_V[0] - this->Iris_V[0]  << "\n\tY = " << this->CP_V[1] - this->Iris_V[1] << "\n\tZ = " << this->CP_V[2] - this->Iris_V[2] << std::endl;

        //Write in txt file
        /*
        std::ofstream file; std::ifstream ifile;

        ifile.open("logs/dropzoneposes.txt", std::ios::ate);
        int str, number, status = 0;
        ifile >> str;
        if (str < 3){
            number = str + 1;
        }
        else if (str == 3){
            status = 1;
        }
        ifile.close();

        if(status == 1)
            file.open("logs/dropzoneposes.txt");
        else
            file.open("logs/dropzoneposes.txt", std::ios::app);
        
        file <<  "Dropzone pose:"<< "\tX = " << CP_V[0] - Iris_V[0]  << "\tY = " << CP_V[1] - Iris_V[1] << "\tZ = " << CP_V[2] - Iris_V[2];
        file << number;
        file.close();*/
    }


    };