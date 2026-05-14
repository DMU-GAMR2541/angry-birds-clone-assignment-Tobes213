
#pragma once
#include "DynamicObject.h"
#include "box2d/box2d.h"
#include <iostream>
#include <set>
/// \brief Custom contact listener to handle collision events in Box2D. Overrides the default contact listener to provide custom behaviour when collisions occur.

// Handles collision detection between physics objects
class ContactListener : public b2ContactListener {

    public:

        // Stores collision IDs
        std::set<uintptr_t> s_ptr;

        // Tracks block collisions
        bool s_blockHit = false;
        b2Body* blockHitBodyA = nullptr;
        b2Body* blockHitBodyB = nullptr;
        uintptr_t hitBlockPtr = 0;

        ContactListener() = default;

    // Called when two fixtures begin to touch
        void BeginContact(b2Contact* contact) override {
            b2Fixture* fixtureA = contact->GetFixtureA();
            b2Fixture* fixtureB = contact->GetFixtureB();

            // Gets collision IDs from both bodies
            uintptr_t ptrA = fixtureA->GetBody()->GetUserData().pointer;
            uintptr_t ptrB = fixtureB->GetBody()->GetUserData().pointer;

            // Detects bird hitting another object
            if (ptrA == 100 && ptrB > 2) {
                s_ptr.insert(ptrB);
                std::cout << ptrA << " and " << ptrB << " hit " << std::endl;
            }
            else if (ptrB == 100 && ptrA > 2) {
                s_ptr.insert(ptrA);
                std::cout << ptrB << " and " << ptrA << " hit " << std::endl;
            }

            // Detects bird hitting a structure block
            if (ptrA == 100 && ptrB >= 200) {
                s_blockHit = true;
                hitBlockPtr = ptrB;
            }
            else if (ptrB == 100 && ptrA >= 200) {
                s_blockHit = true;
                hitBlockPtr = ptrA;
            }
        }
    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact) override {
    }

    // Returns all stored collision IDs
    std::set<uintptr_t> getPointer() {
        return s_ptr;
    }

};