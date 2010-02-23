/****************************************************************************
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

 // Libraries
 #include "BodyState.h"
 #include <cmath>
 #include <iostream>

 // We want to use the ReactPhysics3D namespace
 using namespace reactphysics3d;

 // Constructor
 BodyState::BodyState(const Vector3D& position, const Quaternion& orientation, const Matrix3x3& inertiaTensorInverse, const Kilogram& massInverse)
           : position(position), orientation(orientation), inertiaTensorInverse(inertiaTensorInverse), massInverse(massInverse) {

    // Recalculate the secondary values from the primary values
    recalculate();
}

// Copy-constructor
BodyState::BodyState(const BodyState& bodyState)
          : position(bodyState.position), linearMomentum(bodyState.linearMomentum), orientation(bodyState.orientation),
            angularMomentum(bodyState.angularMomentum), linearVelocity(bodyState.linearVelocity),
            angularVelocity(bodyState.angularVelocity), spin(bodyState.spin), inertiaTensorInverse(bodyState.inertiaTensorInverse),
            massInverse(bodyState.massInverse), externalForce(bodyState.externalForce) {
}

// Destructor
BodyState::~BodyState() {

}

// Recalculate the secondary values of the BodyState when the primary values have changed
void BodyState::recalculate() {

    // Compute the linear velocity
    linearVelocity = linearMomentum * massInverse.getValue();

    // Compute the angular velocity
    angularVelocity = inertiaTensorInverse * angularMomentum;

    // Normalize the orientation quaternion
    orientation = orientation.getUnit();

    // Compute the spin quaternion
    spin = Quaternion(angularVelocity.getX(), angularVelocity.getY(), angularVelocity.getZ(), 0) * orientation * 0.5;
}

// Compute the body state at time t + dt
// This method is used with integration methods that need to compute the state at a future
// time (like Runge-Kutta 4 integrator)
void BodyState::computeAtTime(const Time& timeStep, const DerivativeBodyState& lastDerivativeBodyState) {

    double dt = timeStep.getValue();        // Timestep

    // Compute the position at time t + dt
    position = position + lastDerivativeBodyState.getLinearVelocity() * dt;

    // Compute the linear momentum at time t + dt
    linearMomentum = linearMomentum + lastDerivativeBodyState.getForce() * dt;

    // Compute the orientation at time t + dt
    orientation = orientation + lastDerivativeBodyState.getSpin() * dt;

    // Compute the angular momentum at time t + dt
    angularMomentum = angularMomentum + lastDerivativeBodyState.getTorque() * dt;

    // Recalculate the secondary values of the body state
    recalculate();
}

// Return the force on the body at time t
// This method is used with integration methods that need to compute the state at a future
// time (like Runge-Kutta 4 integrator)
Vector3D BodyState::computeForce(Time time) const {
    // TODO : Implement this method (we want that the user of the library could define this method)
    return externalForce;
}

// Return the torque on the body at time
// This method is used with integration methods that need to compute the state at a future
// time (like Runge-Kutta 4 integrator)
Vector3D BodyState::computeTorque(Time time) const {
    // TODO : Implement this method (we want that the user of the library could define this method)
    return externalTorque;
}
