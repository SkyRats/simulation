#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace gazebo
{
    /**
     * Plugin for a dynamic obstacle.
     * 
     * The model this plugin is attached to will move back and forth a set distance
     * in the X direction.
     */
    class DynamicObstacle: public ModelPlugin
    {
        // Pointer to the model
        private: physics::ModelPtr model;

        // Pointer to the pole link
        private: physics::LinkPtr link;
        
        // Pointer to the update event connection
        private: event::ConnectionPtr updateConnection;

        // Amplitude points
        private: ignition::math::Vector3<double> point_0;
        private: ignition::math::Vector3<double> point_1;
        private: int goal_point = 1;

        // Translation velocity
        private: double vel;
        
        // // PID gains
        // double linear_p = 100.0;
        // double linear_i = 0.0;
        // double linear_d = 0.0;
        // double angular_p = 100.0;
        // double angular_i = 0.0;
        // double angular_d = 0.0;

        // double linear_imax = 123456789.0;
        // double angular_imax = 123456789.0;
        // double maxForce = 123456789.0;
        // double maxTorque = 123456789.0;

        // // PID controllers
        // common::PID controller_x; // Linear x
        // common::PID controller_yy; // Angular y

        // // PID setpoints
        // ignition::math::Vector3<double> targetLinearVel;
        // ignition::math::Vector3<double> targetAngularVel;

        // Called on model creation
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            std::cerr << "Dynamic Obstacle plugin loaded to model [" << _parent->GetName() << "]" << std::endl;
            
            // Store pointers
            this->model = _parent;
            this->link = this->model->GetLink("pole");

            // Distance to oscillate
            double distance = _sdf->Get<double>("distance", 5.0).first;

            // Translation velocity
            this->vel = _sdf->Get<double>("vel");

            // Extreme points
            // this->point_0 = _sdf->Get<ignition::math::Vector3<double>>("point_0",link->WorldCoGPose().Pos());
            this->point_0 = link->WorldCoGPose().Pos();
            this->point_1 = this->point_0 + ignition::math::Vector3<double>(distance, 0.0, 0.0);

            // // Add a PID controller for the DOFs needed
            // controller_x = common::PID(linear_p, linear_i, linear_d, linear_imax, -linear_imax, maxForce, -maxForce);
            // controller_yy = common::PID(angular_p, angular_i, angular_d, angular_imax, -angular_imax, maxTorque, -maxTorque);

            // // Setpoint velocities
            // this->targetLinearVel.X() = 2.0;
            // this->targetAngularVel.Y() = 0.0;

            // Listen to the update event.
            // This event is broadcast every simulation iteration.
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&DynamicObstacle::OnUpdate, this));
        }

         // Called by the world update start event
        public: void OnUpdate()
        {
            // Decide the direction the model should move in
            double v;
            ignition::math::Vector3<double> curr_pos = this->link->WorldCoGPose().Pos();
            if (this->goal_point == 1)
            {
                if (curr_pos.X() <= this->point_1.X())
                {
                    v = this->vel;
                }
                // Overshot point_1, go back
                else
                {
                    v = -this->vel;
                    this->goal_point = 0;
                }
            }
            else
            {
                if (curr_pos.X() >= this->point_0.X()) {
                    v = -this->vel;
                }
                // Overshot point_0, go back
                else
                {
                    v = this->vel;
                    this->goal_point = 1;
                }
            }


            // // Calculate the error between actual and target velocity
            // ignition::math::Vector3<double> curLinearVel = this->link->WorldLinearVel();
            // ignition::math::Vector3<double> curAngularVel = this->link->WorldAngularVel();
            // ignition::math::Vector3<double> linearError = curLinearVel - this->targetLinearVel;
            // ignition::math::Vector3<double> angularError = curAngularVel - this->targetAngularVel;

            // // Get forces to apply from controllers
            // ignition::math::Vector3<double> worldForce;
            // ignition::math::Vector3<double> worldTorque;
            // worldForce.X() = this->controller_x.Update(linearError.X(), dt);
            // worldTorque.Y() = this->controller_yy.Update(angularError.Y(), dt);

            // // Add these forces to the body
            // this->link->AddForce(worldForce);
            // this->link->AddTorque(worldTorque);

            // Apply a small linear velocity to the model.
            this->model->SetLinearVel(ignition::math::Vector3d(v, 0, 0));
            this->model->SetAngularVel(ignition::math::Vector3d(0, 0, 0));
        }
    };

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(DynamicObstacle)
}