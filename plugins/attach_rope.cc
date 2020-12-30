#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"

namespace gazebo
{
class Attach: public WorldPlugin
{
    public: void Load(physics::WorldPtr _world, sdf::ElementPtr /*_sdf*/ )
    {

        physics::JointPtr* joint_1;
        physics::JointPtr* joint_2;
        
        sdf::ElementPtr iris_0 = _world.ModelByName("iris_0");  //esquerda
        sdf::ElementPtr iris_1 =  _world.ModelByName("iris_1"); //direita
        sdf::ElementPtr corda_1 = _world.ModelByName("swarm_box::swarm_box::Corda"); //direita
        sdf::ElementPtr corda_0 = _world.ModelByName("swarm_box::swarm_box::Corda_0"); //esquerda
        //link_10 e o mais em cima da corda
        //base_link do iris

        // Joint_1
        joint_1 = _world.GetPhysicsEngine()->CreateJoint("ball", iris_0);
        joint_1->Load(iris_0.GetLink("base_link"), corda_0.GetLink("link_10"), math::Pose());
        joint_1->Attach(iris_0.GetLink("base_link"), corda_0.GetLink("link_10"));
        joint_1->SetAxis(0, axis_ex);
        joint_1->SetHighStop(0, high_stop_ex);
        joint_1->SetParam("cfm", 0, cfm_ex);

        //Joint_2
        joint_2 = _world.GetPhysicsEngine()->CreateJoint("ball", iris_1);
        joint_2->Load(iris_1.GetLink("base_link"), corda_1.GetLink("link_10"), math::Pose());
        joint_2->Attach(iris_1.GetLink("base_link"), corda_1.GetLink("link_10"));
        joint_2->SetAxis(0, axis_ex);
        joint_2->SetHighStop(0, high_stop_ex);
        joint_2->SetParam("cfm", 0, cfm_ex);
    }
}

    GZ_REGISTER_WORLD_PLUGIN(Attach)
} // namespace gazebo
