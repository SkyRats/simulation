#include "simulation/attach_rope.h"


namespace gazebo {

    void attach_rope::AttachCallback(ConstIntPtr &msg){
        
        
        physics::JointPtr joint_1;
        physics::JointPtr joint_2;

        ignition::math::Vector3d  z_axis = ignition::math::Vector3d(0,0,1);
        ignition::math::Vector3d  null_axis = ignition::math::Vector3d(0,0,0);
        ignition::math::Vector4d  null_axis4D = ignition::math::Vector4d(1,0,0,0);

        //ignition::math::Pose3<ignition::math::Vector3d> nullPose = ignition::math::Pose3<ignition::math::Vector3d>(null_axis, null_axis4D);
        
        physics::ModelPtr corda_0 = this->world->ModelByName("swarm_box::swarm_box::Corda_0"); //esquerda
        physics::ModelPtr iris_0 = this->world->ModelByName("iris_0");  //esquerda
        physics::ModelPtr iris_1 =  this->world->ModelByName("iris_1"); //direita
        physics::ModelPtr corda_1 = this->world->ModelByName("swarm_box::swarm_box::Corda"); //direita
        physics::ModelPtr POSREF = this->world->ModelByName("POS_REF");
        physics::ModelPtr NEGREF = this->world->ModelByName("NEG_REF");
        physics::ModelPtr NULLREF = this->world->ModelByName("NULL_REF");
        physics::ModelPtr FARREF = this->world->ModelByName("FAR_REF");
        physics::ModelPtr spawn1 = this->world->ModelByName("spawn1");
        physics::ModelPtr spawn2 = this->world->ModelByName("spawn2");

        //REMOVER CHAO
        bool f = false;
        bool t = true;
        try{
        spawn1->SetWorldPose(FARREF->WorldPose());
        spawn2->SetWorldPose(FARREF->WorldPose());
        gzmsg << " \tDeleting ground..." << std::endl;
        } catch(std::runtime_error *e) {
            gzmsg << "GROUND REMOVE FAILED" << std::endl;
        }

        gzmsg << "\tAttaching rope..." << std::endl;
        // Joint_1
        joint_1 = this->world->Physics()->CreateJoint("ball", corda_0);
        joint_1->Load(corda_0->GetLink("link_10"), iris_0->GetLink("base_link"), NULLREF->WorldPose());
        joint_1->Attach(corda_0->GetLink("link_10"), iris_0->GetLink("base_link"));
        //joint_1->SetAxis(0, z_axis);
        joint_1->Init();

        //Joint_2 corda_0->GetLink("link_10")->WorldPose()
        joint_2 = this->world->Physics()->CreateJoint("ball", corda_1);
        joint_2->Load(corda_1->GetLink("link_10"), iris_1->GetLink("base_link"), NULLREF->WorldPose());//math::Pose()
        joint_2->Attach(corda_1->GetLink("link_10") ,iris_1->GetLink("base_link"));
        //joint_2->SetAxis(0, z_axis);
        joint_2->Init();
        //link_10 e o mais em cima da corda
        //base_link do iris
        spawn1->SetWorldPose(NULLREF->WorldPose());
        spawn2->SetWorldPose(NULLREF->WorldPose());
    }
    
    void attach_rope::Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf)
    {
        this->world = _parent;

        if (_sdf->HasElement("robotNamespace"))
            namespace_ = _sdf->GetElement("robotNamespace")->Get<std::string>();
        else
            gzerr << "[attach_rope] Please specify a robotNamespace.\n";

        node_handle_ = transport::NodePtr(new transport::Node());
        node_handle_->Init(namespace_);

        attach_sub = node_handle_->Subscribe("~/rope_attach", &attach_rope::AttachCallback, this); //
        gzmsg << "aaaaabriuuuu aqui krai: " << std::endl;
    }
    
}