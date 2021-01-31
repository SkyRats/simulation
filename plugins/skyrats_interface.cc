#include <sstream>
#include <gazebo/msgs/msgs.hh>
#include "simulation/skyrats_interface.hh"

#include <ignition/msgs.hh>

#include "simulation/common.h"

#include <fstream>
#include <math.h>

using namespace gazebo;

// Register this plugin with the simulator
GZ_REGISTER_GUI_PLUGIN(GUIExampleSpawnWidget)


/////////////////////////////////////////////////
GUIExampleSpawnWidget::GUIExampleSpawnWidget()
  : GUIPlugin()
{
  gzmsg << "Hello world" << std::endl;
  this->counter = 0;
  this->wind_status = false;
  this->smoke_status = false;

  // Set the frame background and foreground colors
  this->setStyleSheet(
      "QFrame { background-color : rgba(95, 139, 250, 255); color : blue; }");

  // Create the main layout
  QHBoxLayout *mainLayout = new QHBoxLayout;

  // Create the frame to hold all the widgets
  QFrame *mainFrame = new QFrame();

  // Create the layout that sits inside the frame
  QVBoxLayout *frameLayout = new QVBoxLayout();

  //CREATE BUTTONS
  QPushButton *WindButton = new QPushButton(tr("ON/OFF WIND"));
  connect(WindButton, SIGNAL(clicked()), this, SLOT(WindButton()));

  QPushButton *SmokeButton = new QPushButton(tr("ON/OFF SMOKE"));
  connect(SmokeButton, SIGNAL(clicked()), this, SLOT(SmokeButton()));

  QPushButton *ObstacleButton = new QPushButton(tr("ON/OFF OBSTACLE"));
  connect(ObstacleButton, SIGNAL(clicked()), this, SLOT(ObstacleButton()));

  QPushButton *AttachButton = new QPushButton(tr("Rope Attach"));
  connect(AttachButton, SIGNAL(clicked()), this, SLOT(AttachButton()));

  QPushButton *DropzonePoseButton = new QPushButton(tr("Get Dropzone Poses"));
  connect(DropzonePoseButton, SIGNAL(clicked()), this, SLOT(DropzonePoseButton()));

  //CREATE IMG BOX
  // std::string path = GetPath(thisPath);
  // gzmsg << "a: " << path << std::endl;
  QLabel *Skyrats_txt = new QLabel(("Skyrats Simulator v1.0"));
  // QLabel *Skyrats_img = new QLabel(("/home/felipe/workspaces/workshop/ws/src/simulation/include/simulation/imgs/sky_logo.png"));
  
  // Skyrats_img->setPixmap(QPixmap("/home/felipe/workspaces/workshop/ws/src/simulation/include/simulation/imgs/sky_logo.png"));
  // QPixmap p("/home/felipe/workspaces/workshop/ws/src/simulation/include/simulation/imgs/sky_logo.png");
  //Skyrats_img->setPixmap(p.scaled(Skyrats_img->width(), Skyrats_img->height(), Qt::KeepAspectRatio));
  //Skyrats_img->setScaledContents(true);
  //Skyrats_img->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  //ADD WIDGETS TO INTERFACE
  frameLayout->addWidget(Skyrats_txt);

  //frameLayout->addWidget(Skyrats_img);

  frameLayout->addWidget(WindButton);

  frameLayout->addWidget(SmokeButton);

  frameLayout->addWidget(ObstacleButton);

  QLabel *indoor_txt = new QLabel(("Indoor Simulation"));
  
  frameLayout->addWidget(indoor_txt);

  frameLayout->addWidget(AttachButton);

  frameLayout->addWidget(DropzonePoseButton);

  QLabel *outdoor_txt = new QLabel(("Outdoor Simulation"));
  
  frameLayout->addWidget(outdoor_txt);

  // Add frameLayout to the frame
  mainFrame->setLayout(frameLayout);

  // Add the frame to the main layout
  mainLayout->addWidget(mainFrame);

  // Remove margins to reduce space
  frameLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  this->setLayout(mainLayout);

  // Position and resize this widget
  this->move(10, 10);
  this->resize(160, 265);

  // Create a node for transportation
  this->node = transport::NodePtr(new transport::Node());
  this->node->Init();
  this->factoryPub = this->node->Advertise<msgs::Factory>("~/factory");  

  this->wind_toggle_pub = this->node->Advertise<msgs::Int>("~/wind_toggle");

  this->smoke_pub = this->node->Advertise<msgs::Scene>("~/scene");

  this->obstacle_toggle_pub = this->node->Advertise<msgs::Int>("~/obstacle_toggle");

  this->attach_pub = this->node->Advertise<msgs::Int>("~/rope_attach");

  this->dropzones_pub = this->node->Advertise<msgs::Int>("~/dropzones_pos");
}

/////////////////////////////////////////////////
GUIExampleSpawnWidget::~GUIExampleSpawnWidget(){
}

void GUIExampleSpawnWidget::WindButton()
{
  msgs::Int msg;
  this->wind_status = !this->wind_status;
  msg.set_data(this->wind_status);
  this->wind_toggle_pub->Publish(msg);
  gzmsg << "Wind status: " << wind_status << std::endl;
}

void GUIExampleSpawnWidget::SmokeButton()
{
  msgs::Scene smk_msg;
  msgs::Fog* fog = new msgs::Fog;
  this->smoke_status =! smoke_status;
  smk_msg.set_name("smoke_toggle");
  if (smoke_status == 0){
    smk_msg.clear_fog();

    fog->set_start(10000.0);
    fog->set_density(0.0);
    fog->set_end(30000.0);

    smk_msg.set_allocated_fog(fog);
  }
  if (smoke_status == 1){
    fog->set_start(0.1);
    fog->set_density(300.0);
    fog->set_end(10.0);

    smk_msg.set_allocated_fog(fog);
  }
  
  this->smoke_pub->Publish(smk_msg);
  gzmsg << "Smoke status: " << smoke_status << std::endl;
  gzmsg << "A_Fog status: " << smk_msg.has_fog() << std::endl;
}

void GUIExampleSpawnWidget::ObstacleButton()
{
  msgs::Int obstacle_msg;
  this->obstacle_status = !this->obstacle_status;
  obstacle_msg.set_data(this->obstacle_status);
  this->obstacle_toggle_pub->Publish(obstacle_msg);
  gzmsg << "Obstacle status: " << obstacle_status << std::endl;
}

void GUIExampleSpawnWidget::AttachButton()
{
  gzmsg << "Rope attached message sent..." << std::endl;
  msgs::Int attach_msg;
  attach_msg.set_data(1);
  this->attach_pub->Publish(attach_msg);
}

void GUIExampleSpawnWidget::DropzonePoseButton()
{
  gzmsg << "Sending coordinates..." << std::endl;
  msgs::Int dropzones_msg;
  dropzones_msg.set_data(1);
  this->dropzones_pub->Publish(dropzones_msg);
}




















































        // for(double x = 1; x < 15; x++){
        // msgs::Model model;
        // model.set_name("plugin_unit_sphere_" + std::to_string(this->counter++));
        // msgs::Set(model.mutable_pose(), ignition::math::Pose3d(x/10, x/10, 1.5, 0, 0, 0));
        // const double mass = 1.0;
        // const double radius = x;
        // msgs::AddSphereLink(model, mass, radius);

        // std::ostringstream newModelStr;
        // newModelStr << "<sdf version='" << SDF_VERSION << "'>"
        //   << msgs::ModelToSDF(model)->ToString("")
        //   << "</sdf>";

        // // Send the model to the gazebo server
        // msgs::Factory msg;
        // msg.set_sdf(newModelStr.str());
        // this->factoryPub->Publish(msg);
        // }


        
        //   gazebo::msgs::Vector3d* force = new gazebo::msgs::Vector3d();
        //   force->set_x(3.0 + 2.0);
        //   force->set_y(2.0 + 2.0);
        //   force->set_z(1.0 + 2.0);

        // //  wind_msg.set_frame_id(frame_id_);
        //   wind_msg.set_time_usec(3 * 1e6);
        //   wind_msg.set_allocated_force(force);




        //drone_model->GetLink("NOME DO LINK")->SetForce(ignition::math::Vector3d(1.0, 0.0, 0.0);