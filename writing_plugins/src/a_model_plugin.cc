#include <functional>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>
#include <ros/ros.h>

namespace gazebo {
    class AModelPlugin : public ModelPlugin {
        public:
        void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) {
            // Store the pointer to the model
            this->model = _parent;

            // Set Params
            std::cout << "Load before params" << std::endl;
            this->linear_vel = 0.1;
            if (_sdf->HasElement("linear_vel")) {
                this->linear_vel = _sdf->Get<double>("linear_vel");
            }
            std::cout << this->linear_vel << std::endl;
            this->iterations = 10 * 1000;
            if (_sdf->HasElement("iterations")) {
                this->iterations = _sdf->Get<int>("iterations");
            }
            std::cout << this->iterations << std::endl;

            std::cout << "Load after params" << std::endl;

            // Listen to the update event. This event is broadcast every simulation iteration.
            this->updateConnection = event::Events::ConnectWorldUpdateBegin(
                std::bind(&AModelPlugin::OnUpdate, this));
        }

        // Called by the world update start event
        public:
        void OnUpdate() {
            // Apply a small linear velocity to the model.
            if (this->counter < this->iterations) {
                this->model->SetLinearVel(ignition::math::Vector3d(this->linear_vel, 0, 0));
            } else if (this->counter < (2 * this->iterations)) {
                this->model->SetLinearVel(ignition::math::Vector3d(0, this->linear_vel, 0));
            } else if (this->counter < (3 * this->iterations)) {
                this->model->SetLinearVel(ignition::math::Vector3d(-this->linear_vel, 0, 0));
            } else if (this->counter < (4 * this->iterations)) {
                this->model->SetLinearVel(ignition::math::Vector3d(0, -this->linear_vel, 0));
            } else {
                this->counter = 0;
            }

            this->counter++;
        }

        // Pointer to the model
        private:
        physics::ModelPtr model;

        private:
        int counter;
        double linear_vel;
        int iterations;

        // Pointer to the update event connection
        private:
        event::ConnectionPtr updateConnection;
    };

    // Register this plugin with the simulator
    GZ_REGISTER_MODEL_PLUGIN(AModelPlugin)
} // namespace gazebo