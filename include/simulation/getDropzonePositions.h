#ifndef GET_DROPZONE_POSITION_H
#define GET_DROPZONE_POSITION_H

#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
        static const std::string kDefaultNamespace = "";

    class getDropzonePositions: public ModelPlugin {
        private:
            physics::ModelPtr model;
            physics::LinkPtr link;
            event::ConnectionPtr updateConnection;

            double reference_latitude_;
            double reference_longitude_;
            double reference_heading_;
            double reference_altitude_;

            double radius_north_;
            double radius_east_;

            double CP_V[3];
            double Iris_V[3];

            transport::SubscriberPtr dropzone_sub;
            transport::NodePtr node_handle_;
            std::string namespace_ = "";

    public:
      getDropzonePositions()
      : ModelPlugin(),
        namespace_(kDefaultNamespace){}
            void DropzonesCallback(ConstIntPtr &msg);
            void Setup(sdf::ElementPtr _sdf);
            void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);

    };

    GZ_REGISTER_MODEL_PLUGIN(getDropzonePositions);
}

#endif