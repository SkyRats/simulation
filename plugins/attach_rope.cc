#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"

namespace gazebo
{
class Attach: public WorldPlugin
{

    private: physics::WorldPtr world;

    public: void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
    {
        if(this->world->RealTime().Double() > 50.0){
        this->world = _parent;
        
        physics::JointPtr joint_1;
        physics::JointPtr joint_2;

        ignition::math::Vector3d  z_axis = ignition::math::Vector3d(0,0,1);
        
        physics::ModelPtr iris_0 = this->world->ModelByName("iris_0");  //esquerda
        physics::ModelPtr iris_1 =  this->world->ModelByName("iris_1"); //direita
        physics::ModelPtr corda_1 = this->world->ModelByName("swarm_box::swarm_box::Corda"); //direita
        physics::ModelPtr corda_0 = this->world->ModelByName("swarm_box::swarm_box::Corda_0"); //esquerda
        //link_10 e o mais em cima da corda
        //base_link do iris

        // Joint_1
        joint_1 = this->world->Physics()->CreateJoint("ball", iris_0);
        joint_1->Load(iris_0->GetLink("base_link"), corda_0->GetLink("link_10"), corda_0->GetLink("link_10")->WorldPose());
        joint_1->Attach(iris_0->GetLink("base_link"), corda_0->GetLink("link_10"));
        joint_1->SetAxis(0, z_axis);
        //joint_1->SetHighStop(0, high_stop_ex);
        //joint_1->SetParam("cfm", 0, cfm_ex);
        joint_1->Init();

        //Joint_2
        joint_2 = this->world->Physics()->CreateJoint("ball", iris_1);
        joint_2->Load(iris_1->GetLink("base_link"), corda_1->GetLink("link_10"), corda_0->GetLink("link_10")->WorldPose());//math::Pose()
        joint_2->Attach(iris_1->GetLink("base_link"), corda_1->GetLink("link_10"));
        joint_2->SetAxis(0, z_axis);
        //joint_2->SetHighStop(0, high_stop_ex);
        //joint_2->SetParam("cfm", 0, cfm_ex);
        joint_2->Init();
        }
    }
};

    GZ_REGISTER_WORLD_PLUGIN(Attach);
} // namespace gazebo
