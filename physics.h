#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;

// Conversions for Box2D
const float PI = 3.14159265359;
const float DEG_PER_RAD = 180.0 / PI;
const float PIXELS_PER_METER = 32.0;

typedef b2Body *Block; // Calling a b2Body and "Block" for clarity

namespace physics { // all functions are inline to avoid redeclaration

	// creates a block function

	inline Block createBox(b2World &world, float x, float y, float width, float height, b2BodyType type = b2_dynamicBody) {
		b2BodyDef bodyDef;
		bodyDef.position.Set((x + width / 2.0f) / PIXELS_PER_METER, (y + height / 2.0f) / PIXELS_PER_METER);
		bodyDef.type = type;
		bodyDef.linearDamping = 0.05f;

		b2PolygonShape b2shape;
		b2shape.SetAsBox(width / PIXELS_PER_METER / 2.0f, height / PIXELS_PER_METER / 2.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.restitution = 0.5f;
		fixtureDef.shape = &b2shape;

		b2Body *res = world.CreateBody(&bodyDef);
		res->CreateFixture(&fixtureDef);

		// Create the SFML shape
		sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(width, height));
		shape->setOrigin(width / 2.0f, height / 2.0f);

		// Convert Box2D body position back to SFML pixels for the shape
		b2Vec2 position = res->GetPosition();
		shape->setPosition(position.x * PIXELS_PER_METER, position.y * PIXELS_PER_METER);

		shape->setFillColor(sf::Color::White);

		// Store the shape pointer in the body user data
		res->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape);

		return res;
	}



	inline Block createCircle(b2World &world, float x, float y, float r, b2BodyType type = b2_dynamicBody) {
		b2BodyDef bodyDef;
		bodyDef.position.Set(x / PIXELS_PER_METER, y / PIXELS_PER_METER);
		bodyDef.type = type;
		bodyDef.linearDamping = 0.05F;
		b2CircleShape b2shape;

		b2shape.m_radius = r / PIXELS_PER_METER;

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4;
		fixtureDef.restitution = 0.5;
		fixtureDef.shape = &b2shape;

		b2Body *res = world.CreateBody(&bodyDef);
		res->CreateFixture(&fixtureDef);

		sf::CircleShape *shape1 = new sf::CircleShape(r);
		shape1->setOrigin(r, r);
		shape1->setPosition(x, y);
		shape1->setFillColor(sf::Color::White);

	

		res->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape1);

		return res;
	}

	// Creates a polygon must be between 3 and 8 points with no concavities
	// and points must be specified in a counter clockwise orientation
	inline Block createPolygon(b2World &world, float x, float y, const vector<sf::Vector2f> &pointArray, b2BodyType type = b2_dynamicBody) {
		int n = pointArray.size();
		std::vector<b2Vec2> vertices(n);  // Use std::vector instead of a VLA
		sf::ConvexShape *shape = new sf::ConvexShape(n);

		for (int i = 0; i < n; ++i) {
			vertices[i].Set(pointArray[i].x, pointArray[i].y);
			shape->setPoint(i, pointArray[i]);
		}


		b2PolygonShape polygonShape;
		polygonShape.Set(vertices.data(), n);
		b2BodyDef bodyDef;
		bodyDef.position.Set(x / PIXELS_PER_METER, y / PIXELS_PER_METER);
		bodyDef.type = type;
		bodyDef.linearDamping = 0.05f;

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4;
		fixtureDef.restitution = 0.5;
		fixtureDef.shape = &polygonShape;


		b2Body *res = world.CreateBody(&bodyDef);
		res->CreateFixture(&fixtureDef);

	
		shape->setPosition(x, y);
		shape->setFillColor(sf::Color::White);

		

		res->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape);

		return res;
	}


	inline Block createSprite(b2World &world, float x, float y, string triangleFile, const sf::Texture &texture) {
		size_t n;
		float vx, vy;
		sf::Sprite *shape = new sf::Sprite(texture);

		b2Vec2 vertices[3];
		b2PolygonShape polygonShape;

		b2BodyDef bodyDef;
		bodyDef.position.Set(x / PIXELS_PER_METER, y / PIXELS_PER_METER);
		bodyDef.type = b2_dynamicBody;
		bodyDef.linearDamping = 0.05f;

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4;
		fixtureDef.restitution = 0.5;
		fixtureDef.shape = &polygonShape;


		b2Body *res = world.CreateBody(&bodyDef);

		ifstream myfile;
		myfile.open(triangleFile);
		if (!myfile.is_open()) {
			cout << "No vertex file located" << std::endl;
		}
		else {
			cout << "Vertex file opened successfully.\n";
		}

		myfile >> n;
		cout << n << endl;

		/*myfile >> n;
		cout << n << endl;*/

		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < 3; j++) {
				myfile >> vx;
				myfile >> vy;
				vertices[j].Set(vx / PIXELS_PER_METER, vy / PIXELS_PER_METER);
			}
			polygonShape.Set(vertices, 3);
			fixtureDef.shape = &polygonShape;
			res->CreateFixture(&fixtureDef);
		}

		shape->setPosition(x, y);
		
		res->GetUserData().pointer = reinterpret_cast<uintptr_t>(shape);


		myfile.close();
		return res;

	}

	// function that simulates physics, renders shapes, and steps through one time iteration
	inline void displayWorld(b2World &world, sf::RenderWindow &render) {
		world.Step(1.0 / 60, int32(8), int32(3));
		for (b2Body *body = world.GetBodyList(); body != 0; body = body->GetNext()) {
			
			sf::Shape *shape = reinterpret_cast<sf::Shape *>(body->GetUserData().pointer);

			shape->setPosition(body->GetPosition().x * PIXELS_PER_METER, body->GetPosition().y * PIXELS_PER_METER);
			shape->setRotation(body->GetAngle() * DEG_PER_RAD);
			render.draw(*shape);
		}
	}

	// function that sets a texture to a block object
	inline void setBlockTexture(Block b, const sf::Texture *t) {
		sf::Shape *shape = reinterpret_cast<sf::Shape *>(b->GetUserData().pointer);
		shape->setTexture(t);
	}


	// sets Block color
	inline void setBlockColor(Block b, sf::Color c) {
		sf::Shape *shape = reinterpret_cast<sf::Shape *>(b->GetUserData().pointer);
		shape->setFillColor(c);
	}

	// Function that checks collision between two blocks
	inline bool checkCollision(Block b1, Block b2) {
		for (b2ContactEdge *edge = b1->GetContactList(); edge; edge = edge->next)
		{
			if (edge->other == b2) {
				if (edge->contact->IsTouching()) return true;
			}
		}
		return false;
	}

	// Checks collision of Block b and any other Block
	inline bool checkCollision(Block b) {
		for (b2ContactEdge *edge = b->GetContactList(); edge; edge = edge->next) {
			if (edge->contact->IsTouching()) return true;
		}
		return false;
	}

	// Function that sets a Block collision id
	// by default all Blocks have an id of 0
	// id rules:
	// if two Blocks have an id of 0 they will collide
	// if two Blocks have different id's they will collide
	// if two Blocks have the same positive id they will collide
	// if two Blocks have the same negative id they will not collide
	inline void setCollisionId(Block b, int16 id) {
		b2Filter filter;
		for (b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext()) {
			filter = f->GetFilterData();
			filter.groupIndex = id;
			f->SetFilterData(filter);
		}
	}

	// Moves Block b according to Vector force
	inline void moveBody(Block b, sf::Vector2f force) {
		b->ApplyLinearImpulseToCenter(b2Vec2(force.x, force.y), 1);
	}

	// Spins Block B with an Amplitude (negative amp is counterclockwise)
	inline void spin(Block b, float amp) {
		b->ApplyAngularImpulse(amp, 1);
	}

	// Turns off spin for Block B
	inline void noSpin(Block b) {
		b->SetFixedRotation(true);
	}

	inline void noGravity(Block b) {
		b->SetGravityScale(0.0);
	}

	inline void resetGravity(Block b) {
		b->SetGravityScale(1.0);
	}

	// returns the velocity of Block b as a Vector
	inline sf::Vector2f getVelocity(Block b) {
		b2Vec2 vel = b->GetLinearVelocity();
		sf::Vector2f sfmlVel(vel.x * PIXELS_PER_METER, vel.y * PIXELS_PER_METER);
		return sfmlVel;
	}

	// function that sets the velocity of a block
	inline void setVelocity(Block b, sf::Vector2f vel) {
		b->SetLinearVelocity(b2Vec2(vel.x / PIXELS_PER_METER, vel.y / PIXELS_PER_METER));
	}

	// function that keeps the angle of the Block but
	// limits its speed
	inline void limitVelocity(Block b, float speed) {
		b2Vec2 velocity = b->GetLinearVelocity();
		b->SetLinearVelocity((speed / PIXELS_PER_METER) / velocity.Length() * velocity);
	}


	inline sf::Vector2f getPosition(Block b) {
		sf::Vector2f sfmlPos(b->GetPosition().x * PIXELS_PER_METER, b->GetPosition().y * PIXELS_PER_METER);
		return sfmlPos;
	}

	inline float getAngle(Block b) {
		return b->GetAngle();
	}

	inline float getAngleSpeed(Block b) {
		return b->GetAngularVelocity();
	}

	inline void deleteBlock(b2World &world, Block b) {
		world.DestroyBody(b);
		b->SetUserData(0);
	}

	inline bool isDestroyed(Block b) {
		if (b->GetUserData().pointer == 0) return true;
		else return false;
	}

	inline void setGravity(b2World &w, sf::Vector2f g) {
		w.SetGravity(b2Vec2(g.x, g.y));
	}

	// returns the memory address of the SFML shape of a block
	inline sf::Shape *getSFMLShape(Block b) {
		return reinterpret_cast<sf::Shape *>(b->GetUserData().pointer);
	}


};