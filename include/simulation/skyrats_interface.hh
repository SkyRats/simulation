/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _GUI_EXAMPLE_SPAWN_WIDGET_HH_
#define _GUI_EXAMPLE_SPAWN_WIDGET_HH_

#include <gazebo/common/Plugin.hh>
#include <gazebo/gui/GuiPlugin.hh>


#include <ignition/math/Vector3.hh>
#include "gazebo/common/Event.hh"
#include "gazebo/common/Plugin.hh"
#include "gazebo/physics/physics.hh"
#include "gazebo/sensors/Noise.hh"

#include <gazebo/msgs/msgs.hh>

#include <gazebo/rendering/rendering.hh>

// moc parsing error of tbb headers
#ifndef Q_MOC_RUN
#include <gazebo/transport/transport.hh>
#endif

namespace gazebo
{
    class GAZEBO_VISIBLE GUIExampleSpawnWidget : public GUIPlugin
    {
      Q_OBJECT

      /// \brief Constructor
      /// \param[in] _parent Parent widget
      public: GUIExampleSpawnWidget();

      /// \brief Destructor
      public: virtual ~GUIExampleSpawnWidget();

      ///BOTOES
      protected slots: void OnButton();

      protected slots: void WindButton();

      protected slots: void SmokeButton();

      protected slots: void ObstacleButton();

      /// \brief Counter used to create unique model names
      private: unsigned int counter;

      /// \brief Node used to establish communication with gzserver.
      private: transport::NodePtr node;

      /// \brief Publisher of factory messages.
      private: transport::PublisherPtr factoryPub;

      private: transport::PublisherPtr wind_toggle_pub;

      private: transport::PublisherPtr smoke_pub;

      private: transport::PublisherPtr obstacle_toggle_pub;

      private: bool wind_status;

      private: bool smoke_status;

      private: bool obstacle_status;
    };
}
#endif
