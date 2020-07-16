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

        // Point the model is moving to
        private: int goal_point = 1;

        // Unit vector from point_0 to point_1
        private: ignition::math::Vector3<double> direction;

        // Distance from point_0 to point_1
        private: double distance;

        // Translation velocity
        private: double vel;

        // Called on model creation
        public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
        {
            std::cout << "Dynamic Obstacle plugin loaded to model [" << _parent->GetName() << "]" << std::endl;

            // Store pointers
            this->model = _parent;
            this->link = this->model->GetLink("pole");

            // Translation velocity
            this->vel = _sdf->Get<double>("vel");

            // Extreme points
            this->point_0 = _sdf->Get<ignition::math::Vector3<double>>("point_0", link->WorldCoGPose().Pos()).first;
            this->point_1 = _sdf->Get<ignition::math::Vector3<double>>("point_1", this->point_0 + ignition::math::Vector3<double>(5.0, 0.0, 0.0)).first;

            this->direction = (this->point_1 - this->point_0).Normalized();
            this->distance = this->point_0.Distance(point_1);

            // Listen to the update event.
            // This event is broadcast every simulation iteration.
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&DynamicObstacle::OnUpdate, this));
        }

         // Called by the world update start event
        public: void OnUpdate()
        {
            double v;
            ignition::math::Vector3<double> curr_pos = this->link->WorldCoGPose().Pos();
            // Coordinate of the model in the direction between point_0 and point_1
            double x_direction = (curr_pos - this->point_0).Dot(this->direction);
            
            // Decide the direction the model should move in
            if (this->goal_point == 1)
            {
                if (x_direction <= this->distance)
                {
                    v = this->vel;
                }
                // Reached point_1, go back
                else
                {
                    v = -this->vel;
                    this->goal_point = 0;
                }
            }
            else
            {
                if (x_direction >= 0) {
                    v = -this->vel;
                }
                // Reached point_0, go back
                else
                {
                    v = this->vel;
                    this->goal_point = 1;
                }
            }

            // Apply linear velocity to the model in the correct direction.
            this->model->SetLinearVel(v*this->direction);
            this->model->SetAngularVel(ignition::math::Vector3d(0, 0, 0));
        }
    };

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(DynamicObstacle)
}