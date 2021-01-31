#include "../include/simulation/getDropzonePositions.h"

static const double equatorial_radius = 6378197.0;
static const double flattening = 1.0/298.257223563;
static const double excentrity2 = 2*flattening - flattening*flattening;

static const double DEFAULT_REFERENCE_LATITUDE = 49.9;
static const double DEFAULT_REFERENCE_LONGITUDE = 8.9;
static const double DEFAULT_REFERENCE_HEADING   = 0.0;
static const double DEFAULT_REFERENCE_ALTITUDE  = 0.0;

namespace gazebo
{
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

        _sdf->GetElement("latitude_deg")->GetValue()->Get(reference_latitude_);
        _sdf->GetElement("longitude_deg")->GetValue()->Get(reference_longitude_);
        _sdf->GetElement("heading_deg")->GetValue()->Get(reference_heading_);
        reference_heading_ *= M_PI/180.0;

        double temp = 1.0 / (1.0 - excentrity2 * sin(reference_latitude_ * M_PI/180.0) * sin(reference_latitude_ * M_PI/180.0));
        double prime_vertical_radius = equatorial_radius * sqrt(temp);
        radius_north_ = prime_vertical_radius * (1 - excentrity2) * temp;
        radius_east_  = prime_vertical_radius * cos(reference_latitude_ * M_PI/180.0);
    }

    // Called on model creation
    void getDropzonePositions::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
    {
        Setup(_sdf);
        ignition::math::Pose3d dz_clean_pose = _parent->WorldPose();
        ignition::math::Vector3<double> dzc_p = dz_clean_pose.Pos();
        double CP_V[3] = {dzc_p.X(), dzc_p.Y(), dzc_p.Z() };

        physics::WorldPtr puebla = _parent->GetWorld();
        physics::ModelPtr iris = puebla->ModelByName("iris");
        ignition::math::Pose3d iris_pose = iris->WorldPose();
        ignition::math::Vector3<double> iris_p = iris_pose.Pos();
        double Iris_V[3] = {iris_p.X(), iris_p.Y(), iris_p.Z() };

        gzmsg << "Dropzone GPS:\n\t Latitude = " << reference_latitude_  + ( cos(reference_heading_) * CP_V[0] + sin(reference_heading_) * CP_V[1]) / radius_north_ * 180.0/M_PI << "\n\t Longitude = " << reference_longitude_ - (-sin(reference_heading_) * CP_V[0] + cos(reference_heading_) * CP_V[1]) / radius_east_  * 180.0/M_PI << "\n"<< std::endl;
        gzmsg << "Dropzone pose:"<< "\n\tX = " << CP_V[0] - Iris_V[0]  << "\n\tY = " << CP_V[1] - Iris_V[1] << "\n\tZ = " << CP_V[2] - Iris_V[2] << std::endl;
    }
    };