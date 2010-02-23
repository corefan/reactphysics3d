/***************************************************************************
* Copyright (C) 2009      Daniel Chappuis                                  *
****************************************************************************
* This file is part of ReactPhysics3D.                                     *
*                                                                          *
* ReactPhysics3D is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or     *
* (at your option) any later version.                                      *
*                                                                          *
* ReactPhysics3D is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU Lesser General Public License for more details.                      *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public License *
* along with ReactPhysics3D. If not, see <http://www.gnu.org/licenses/>.   *
***************************************************************************/

 #ifndef RIGIDBODY_H
 #define RIGIDBODY_H

 // Libraries
 #include <cassert>
 #include "Body.h"
 #include "BodyState.h"
 #include "OBB.h"
 #include "../mathematics/mathematics.h"
 #include "../physics/physics.h"

// Namespace reactphysics3d
namespace reactphysics3d {

// TODO : Check if it is possible to put all the attributes from BodyState class in the RigidBody class (to do this, the
//        class RungeKutta have to be changed a lot

/*  -------------------------------------------------------------------
    Class RigidBody :
        This class represents a rigid body of the physics
        engine. A rigid body is a non-deformable body that
        has a constant mass.
    -------------------------------------------------------------------
*/
class RigidBody : public Body {
    private :
        Matrix3x3 inertiaTensor;                    // Inertia tensor of the body
        BodyState currentBodyState;                 // Current body state
        BodyState previousBodyState;                // Previous body state
        bool isMotionEnabled;                       // True if the body can move
        bool isCollisionEnabled;                    // True if the body can collide with others bodies
        double interpolationFactor;                 // Interpolation factor used for the state interpolation
        OBB obb;                                    // Oriented bounding box that contains the rigid body

    public :
        RigidBody(const Vector3D& position, const Quaternion& orientation, const Kilogram& mass, const Matrix3x3& inertiaTensor, const OBB& obb);   // Constructor
        RigidBody(const RigidBody& rigidBody);                                                                                                      // Copy-constructor
        virtual ~RigidBody();                                                                                                                       // Destructor

        Matrix3x3 getInertiaTensor() const;                             // Return the inertia tensor of the body
        void setInertiaTensor(const Matrix3x3& inertiaTensor);          // Set the inertia tensor of the body
        BodyState& getCurrentBodyState();                               // Return a reference to the current state of the body
        BodyState& getPreviousBodyState();                              // TODO : DELETE THIS
        void setInterpolationFactor(double factor);                     // Set the interpolation factor of the body
        BodyState getInterpolatedState() const;                         // Compute and return the interpolated state
        bool getIsMotionEnabled() const;                                // Return if the rigid body can move
        void setIsMotionEnabled(bool isMotionEnabled);                 // Set the value to true if the body can move
        void setLinearVelocity(const Vector3D& linearVelocity);         // Set the linear velocity of the rigid body
        void updatePreviousBodyState();                                 // Update the previous body state of the body
        OBB getOBB() const;                                             // Return the oriented bounding box of the rigid body
        void update();                                                  // Update the rigid body in order to reflect a change in the body state
};

// --- Inline functions --- //

// Return the inertia tensor of the body
inline Matrix3x3 RigidBody::getInertiaTensor() const {
    return inertiaTensor;
}

// Set the inertia tensor of the body
inline void RigidBody::setInertiaTensor(const Matrix3x3& inertiaTensor) {
    this->inertiaTensor = inertiaTensor;
}

// Return a reference to the current state of the body
// This way the currentBodyState could be modify outside the rigid body
inline BodyState& RigidBody::getCurrentBodyState() {
    return currentBodyState;
}

// TODO : DELETE THIS
inline BodyState& RigidBody::getPreviousBodyState() {
    return previousBodyState;
}


// Set the interpolation factor of the body
inline void RigidBody::setInterpolationFactor(double factor) {
    assert(factor >= 0 && factor <= 1);

    // Set the factor
    interpolationFactor = factor;
}

// Return if the rigid body can move
inline bool RigidBody::getIsMotionEnabled() const {
    return isMotionEnabled;
}

// Set the value to true if the body can move
inline void RigidBody::setIsMotionEnabled(bool isMotionEnabled) {
    this->isMotionEnabled = isMotionEnabled;
}

// Set the linear velocity of the rigid body
inline void RigidBody::setLinearVelocity(const Vector3D& linearVelocity) {
    // Update the linear velocity of the current body state
    currentBodyState.setLinearVelocity(linearVelocity);

    // Update the linear momentum of the current body state
    currentBodyState.setLinearMomentum(linearVelocity * (1.0/currentBodyState.getMassInverse().getValue()));
}

// Update the previous body state of the body
inline void RigidBody::updatePreviousBodyState() {
    // The current body state becomes the previous body state
    previousBodyState = currentBodyState;
}

// Return the oriented bounding box of the rigid body
inline OBB RigidBody::getOBB() const {
    return obb;
}

// Update the rigid body in order to reflect a change in the body state
inline void RigidBody::update() {
    // Update the orientation of the corresponding bounding volume of the rigid body
    obb.updateOrientation(currentBodyState.getPosition(), currentBodyState.getOrientation());
}

} // End of the ReactPhyscis3D namespace

 #endif
