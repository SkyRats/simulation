#include <iostream>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo
{
    class RopePlugin : public WorldPlugin
    {
    private:
        // Pointer to the world
        physics::WorldPtr world;

        // Pointer to the models
        physics::ModelPtr model1, model2;
        physics::LinkPtr model1_link, model2_link;
        
        // Model positions
        ignition::math::Vector3<double> model1_pos, model2_pos;

        // Rope length
        double length;

        // Rope state
        bool slack;

        // Rope tension vector
        ignition::math::Vector3<double> tension;

        // Pointer to the update event connection
        event::ConnectionPtr updateConnection;

    public:
        void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
        {
            
            this->world = _world;
            std::cout << "[rope_plugin] rope_plugin loaded to world" << std::endl;

            // Get models
            std::string model1_name = _sdf->Get<std::string>("model1");
            std::string model2_name = _sdf->Get<std::string>("model2");
            this->model1 = this->world->ModelByName(model1_name);
            this->model2 = this->world->ModelByName(model2_name);
            
            // Model links
            this->model1_link = this->model1->GetLinks()[0];
            this->model2_link = this->model2->GetLinks()[0];

            std::cout << "[rope_plugin] Model1: " << this->model1->GetName() << std::endl;
            std::cout << "[rope_plugin] Model2: " << this->model2->GetName() << std::endl;            

            // Rope length
            this->length = _sdf->Get<double>("length");

            // Listen to the update event
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(std::bind(&RopePlugin::OnUpdate, this));
        }

        void OnUpdate()
        {
            this->model1_pos = this->model1->WorldPose().Pos();
            this->model2_pos = this->model2->WorldPose().Pos();

            this->UpdateTension();

            this->model1_link->AddForce(this->tension);
            this->model2_link->AddForce(-this->tension);
        }

        void UpdateTension()
        {
            double distance = this->model1_pos.Distance(this->model2_pos);
            
            if (distance < this->length) {
                std::cout << "[rope_plugin] Slack (distance = " << distance << ")" << std::endl;
                this->tension = ignition::math::Vector3<double>(0.0, 0.0, 0.0);
            } else {
                std::cout << "[rope_plugin] Taut (distance = " << distance << ")" << std::endl;
                this->tension = 100*(distance - this->length)*(this->model2_pos - this->model1_pos).Normalized();
            }

        }
    };

    GZ_REGISTER_WORLD_PLUGIN(RopePlugin)
} // namespace gazebo