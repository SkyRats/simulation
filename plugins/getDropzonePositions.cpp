#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
    class getDropzonePositions: public ModelPlugin
    {
        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer to the pole link
        private: physics::LinkPtr link;
        
        // Pointer to the update event connection
        private: event::ConnectionPtr updateConnection;

        // Called on model creation
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            ignition::math::Pose3d dz_clean_pose = _parent->WorldPose();
            ignition::math::Vector3<double> dzc_p = dz_clean_pose.Pos();
            double CP_V[3] = {dzc_p.X(), dzc_p.Y(), dzc_p.Z() };
            gzmsg << "Dropzone pose:"<< "\n\tX = " << CP_V[0] << "\n\tY = " << CP_V[1] << "\n\tZ = " << CP_V[2] << std::endl;
        }
    };
    GZ_REGISTER_MODEL_PLUGIN(getDropzonePositions)
}