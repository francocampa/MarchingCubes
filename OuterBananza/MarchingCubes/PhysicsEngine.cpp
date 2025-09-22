#include "PhysicsEngine.h"
#include "Player.h"
#include "InputController.h"
#include <glm/gtx/vector_angle.hpp> // for glm::angle

PhysicsEngine::PhysicsEngine() {
	settings.gravity = Vector3(gravity.x,gravity.y,gravity.z);
	world = physicsCommon.createPhysicsWorld(settings);
}

void PhysicsEngine::processPhysics(float delta)
{
	timer += delta;
	if (timer < timeStep)
		return;

	timer -= timeStep;

	world->update(timeStep);

	Transform playerTransform = playerBody->getTransform();
	Vector3 playerPos = playerTransform.getPosition();
	player->moveToPos({ playerPos.x,playerPos.y,playerPos.z});

}

void PhysicsEngine::addTerrain(Mesh m)
{
	if (m.vertices.size() == 0)
		return;
	TriangleVertexArray vertexArray(
		m.vertices.size(),                        
		&m.vertices[0], sizeof(float) * 6,
		m.indices.size() / 3,                    
		&m.indices[0], sizeof(int) * 3,
		TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
		TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
	);

	std::vector<rp3d::Message> messages;

	// Create the collision shape from the triangle mesh
	TriangleMesh* triangleMesh = physicsCommon.createTriangleMesh(vertexArray,messages);
	const Vector3 scaling(1, 1, 1);
	ConcaveMeshShape* terrainShape = physicsCommon.createConcaveMeshShape(triangleMesh, scaling);

	// Create static rigidbody for terrain
	RigidBody* terrainBody = world->createRigidBody(Transform(Vector3(0, 0, 0), Quaternion::identity()));
	Collider* terrainCollider = terrainBody->addCollider(terrainShape, Transform::identity());
	rp3d::Material& terrainMat = terrainCollider->getMaterial();
	terrainMat.setBounciness(0.0f);
	terrainMat.setFrictionCoefficient(0.9f);
	terrainBody->setType(BodyType::STATIC);
	terrainBodies.push_back(terrainBody);
}

void PhysicsEngine::setPlayer(Player* player)
{
	float radius = player->getRadius();

	// Create sphere shape
	SphereShape* sphereShape = physicsCommon.createSphereShape(radius);

	// Initial position of the player
	Transform playerTransform(Vector3(0, 5, 0), Quaternion::identity());

	playerBody = world->createRigidBody(playerTransform);

	// Attach collider
	Collider* playerCollider = playerBody->addCollider(sphereShape, Transform::identity());
	rp3d::Material& playerMat = playerCollider->getMaterial();
	playerMat.setBounciness(0.0f);      
	playerMat.setFrictionCoefficient(0.8f);

	// Dynamic so it moves
	playerBody->setType(BodyType::DYNAMIC);

	// Optional: tweak mass & inertia
	decimal mass = 1.0f;
	playerBody->updateMassPropertiesFromColliders();
	player->setBody(playerBody);
	this->player = player;
}

