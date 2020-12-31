#include <ignition/math/Pose3.hh>
#include "gazebo/physics/physics.hh"
#include "gazebo/common/common.hh"
#include "gazebo/gazebo.hh"

namespace gazebo
{
    static const std::string kDefaultNamespace = "";
class attach_rope: public WorldPlugin
{

    private:   
        physics::WorldPtr world;
        transport::SubscriberPtr attach_sub;
        transport::NodePtr node_handle_;
        std::string namespace_;

    public:
      attach_rope()
      : WorldPlugin(),
        namespace_(kDefaultNamespace){}
        void AttachCallback(ConstIntPtr &msg);
        void Load(physics::WorldPtr _parent, sdf::ElementPtr _sdf);
    
        
};

    GZ_REGISTER_WORLD_PLUGIN(attach_rope);
} // namespace gazebo
