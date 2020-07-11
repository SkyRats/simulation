#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
    class DynamicObstacle: public ModelPlugin
    {
        //Pointer to the model
        private: physics::ModelPtr model;
        // Pointer to the update event connection
        private: event::ConnectionPtr updateConnection;

        // Called on model creation
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            std::cerr << "Dynamic Obstacle plugin loaded to model [" << _parent->GetName() << "]" << std::endl;
            
            // Store the pointer to the model
            this->model = _parent;

            //Apply a small linear velocity to the model.
            this->model->SetLinearVel(ignition::math::Vector3d(2.0, 0, 0));
            this->model->SetAngularVel(ignition::math::Vector3d(0, 0, 0));

            // Listen to the update event.
            // This event is broadcast every simulation iteration.
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&DynamicObstacle::OnUpdate, this));
        }

         // Called by the world update start event
        public: void OnUpdate()
        {
            //Apply a small linear velocity to the model.
            this->model->SetLinearVel(ignition::math::Vector3d(1.0, 0, 0));
            this->model->SetAngularVel(ignition::math::Vector3d(0, 0, 0));
        }
    };

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(DynamicObstacle)
}