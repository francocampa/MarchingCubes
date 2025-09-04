#include "TerrainGenerator.h"


void TerrainGenerator::handleCamera(float delta)
{
	if (InputController::mouseRight) {
		//Rotate
		SDL_SetRelativeMouseMode(SDL_TRUE);
		float sensitivity = 0.5f;
		float theta = cam.theta + sensitivity * delta * InputController::mouseMove.x;
		float phi = cam.phi + sensitivity * sin(delta * InputController::mouseMove.y);

		float epsilon = 0.5f;
		if (phi < epsilon) phi = epsilon;
		if (phi > M_PI - epsilon) phi = M_PI - epsilon;

		cam.theta = theta;
		cam.phi = phi;
	}
	else if (InputController::mouseWheel) {
		float vel = 3.0f * delta;
		glm::vec3 front = glm::normalize(cam.position - center);
		glm::vec3 right = glm::normalize(glm::cross(front, { 0,1,0 }));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		right *= InputController::mouseMove.x * vel;
		up *= InputController::mouseMove.y * vel;

		cam.position += right + up;
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (!InputController::mouseWheel) {
		float vel = 10.0f;
		glm::vec3 front = glm::normalize(cam.position - center);
		glm::vec3 right = glm::normalize(glm::cross(front, { 0,1,0 }));
		if (InputController::w) {
			cam.position = cam.position - vel * delta * front;
		}
		if (InputController::s) {
			cam.position = cam.position + vel * delta * front;
		}
		if (InputController::d) {
			cam.position = cam.position - vel * delta * right;
		}
		if (InputController::a) {
			cam.position = cam.position + vel * delta * right;
		}
	}

	center.x = cam.position.x + sin(cam.phi) * cos(cam.theta);
	center.y = cam.position.y + cos(cam.phi);
	center.z = cam.position.z + sin(cam.phi) * sin(cam.theta);

	gluLookAt(cam.position.x, cam.position.y, cam.position.z, center.x, center.y, center.z, 0, 1, 0);
}

void TerrainGenerator::handleLight()
{
	if (enabledLight)
		return;

	glEnable(GL_LIGHT0);

	GLfloat lightPos[] = { 0.0f, 20.0f, 0.0f, 0.0f };
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	enabledLight = true;
}

void TerrainGenerator::handlePlayer(float delta)
{
	player.process(delta);

	glm::vec3 pPos = player.getPos();

	glm::vec3 chunkPos = getChunkForPos(pPos);

	glm::vec3 chunkDiff = glm::abs(chunkPos - centerChunk);

	if (chunkDiff.x > 0.1f || chunkDiff.y > 0.1f || chunkDiff.z > 0.1f) {
		centerChunk = chunkPos;
		printVec3(chunkPos);
		generateTerrain();
	}
}

void TerrainGenerator::handleUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Files menu");
	;
	if (ImGui::SliderFloat("Threshold", &threshold, -5.0f, 5.0f)
		|| ImGui::SliderFloat("Frequency", &frequency, 0.0f, 1.0f)
		|| ImGui::InputInt("Seed", &seed)) {
		setNoiseParams();
		generateTerrain();
	}

	// Keep track of current selection
	int currentNoiseType = noiseType;

	// Labels for display in the combo
	const char* noiseTypeNames[] = {
		"OpenSimplex2",
		"OpenSimplex2S",
		"Cellular",
		"Perlin",
		"ValueCubic",
		"Value"
	};

	// Combo box
	if (ImGui::BeginCombo("Noise Type", noiseTypeNames[currentNoiseType]))
	{
		for (int n = 0; n < IM_ARRAYSIZE(noiseTypeNames); n++)
		{
			bool isSelected = (currentNoiseType == n);
			if (ImGui::Selectable(noiseTypeNames[n], isSelected))
			{
				currentNoiseType = n;
				noiseType = (FastNoiseLite::NoiseType)currentNoiseType;
				setNoiseParams();
				generateTerrain();
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Checkbox("Show spheres", &showSpheres);
	ImGui::Checkbox("Show chunk box", &showChunks);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TerrainGenerator::handleVisualizations()
{
	if (showSpheres)
		for (int x = 0; x < base; x++)
			for (int y = 0; y < height; y++)
				for (int z = 0; z < base; z++)
				{
					Sphere s = visualizationSpheres[x * base * height + y * base + z];
					renderMesh(s.m, s.pos, s.color, 1);
				}

	if (showChunks) {
		float chunkSeparation = base * distance;
		for (int x = 0; x < 3; x++)
			for (int z = 0; z < 3; z++) {
				glm::vec3 chunkOrigin = centerChunk - glm::vec3(x - 1, 0, z - 1) * chunkSeparation;
				Mesh cube = createCubeMesh(chunkSeparation);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				renderMesh(cube, chunkOrigin + base * distance / 2, glm::vec3(1, 1, 1));
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		
	}
}

glm::vec3 TerrainGenerator::getChunkForPos(glm::vec3 pos)
{
	glm::vec3 chunkSize = glm::vec3(base, height, base)*distance;

	int chunkX = static_cast<int>(floor(pos.x / chunkSize.x));
	int chunkY = static_cast<int>(floor(pos.y / chunkSize.y));
	int chunkZ = static_cast<int>(floor(pos.z / chunkSize.z));

	glm::vec3 chunkOrigin = glm::vec3(
		chunkX * chunkSize.x,
		chunkY * chunkSize.y,
		chunkZ * chunkSize.z
	);

	return chunkOrigin;
}

void TerrainGenerator::setNoiseParams()
{
	noise.SetNoiseType(noiseType);
	noise.SetFrequency(frequency);
	noise.SetSeed(seed);
}

TerrainGenerator::TerrainGenerator()
{
	glShadeModel(GL_SMOOTH);
	setNoiseParams();
	generateTerrain();

	player = Player();
}

void TerrainGenerator::generateTerrain()
{
	float chunkSeparation = base * distance;

	for (int x = 0; x < 3; x++)
		for (int z = 0; z < 3; z++) {
			glm::vec3 chunkOrigin = centerChunk - glm::vec3(x-1,0,z-1) * chunkSeparation;
			int index = x * 3 + z;
			chunks[index].mesh.vertices.clear();
			chunks[index].mesh.indices.clear();
			generateChunk(chunkOrigin,chunks[index].mesh);
			chunks->pos = chunkOrigin;
		}
}

void TerrainGenerator::generateChunk(glm::vec3 offset, Mesh&terrain) {
	visualizationSpheres.clear();
	// Preview spheres
	for (int x = 0; x < base; x++) 
		for (int y = 0; y < height; y++) 
			for (int z = 0; z < base; z++) {
				glm::vec3 pos = offset + glm::vec3(x,y,z) * distance;

				Sphere s;
				s.m = createSphereMesh(0.05f, 6, 6);
				s.pos = pos;
				s.color = f(pos) > threshold ? glm::vec3(1, 1, 1) : glm::vec3(0, 0, 0);
				visualizationSpheres.push_back(s);
			}


	//MC
	for (int x = 0; x < base; x++)
		for (int y = 0; y < height; y++)
			for (int z = 0; z < base; z++) {
				glm::vec3 pos = offset + glm::vec3(x, y, z) * distance;
				int mcCase = 0;
				for (int i = 0; i < 8; i++)
					if (f(getIndexPos(pos,i)) > threshold)
						mcCase |= (1 << i);

				for (int i = 0; i < 13; i = i + 3) {
					if (TriangleTable[mcCase][i] == -1)
						break;

					glm::vec3 points[3];
					for (int j = 0; j < 3; j++)
					{
						int edge = TriangleTable[mcCase][i + j];
						glm::vec3 corner0 = getIndexPos(pos, EdgeVertexIndices[edge][0]);
						glm::vec3 corner1 = getIndexPos(pos, EdgeVertexIndices[edge][1]);
						float corner0Val = f(corner0);
						float corner1Val = f(corner1);
						float t = (threshold - corner0Val) / (corner1Val - corner0Val);
						glm::vec3 point = corner0 + t * (corner1 - corner0);

						points[j] = point;
					}

					glm::vec3 e0 = points[1] - points[0];
					glm::vec3 e1 = points[2] - points[0];
					glm::vec3 normal = glm::cross(e1, e0);
					for (int j = 0; j < 3; j++) {
						Vertex v = { points[j], normal ,glm::vec2(0,0) };
						terrain.indices.push_back(terrain.vertices.size());
						terrain.vertices.push_back(v);
					}
				}
			}
}

float TerrainGenerator::f(glm::vec3 pos) {
	return -pos.y + noise.GetNoise(pos.x, pos.y, pos.z);
}

void TerrainGenerator::process(float delta) {
	handleCamera(delta);
	handleLight();
	handlePlayer(delta);
	handleVisualizations();

	glEnable(GL_LIGHTING);
	
	for(int i = 0; i < 9 ; i++)
		renderMesh(chunks[i].mesh, glm::vec3(0, 0, 0), glm::vec3(0.5, 0, 0), 1);

	glDisable(GL_LIGHTING);

	handleUI();
}


glm::vec3 TerrainGenerator::getIndexPos(glm::vec3 offset, int i)
{
	glm::vec3 pos = glm::vec3(
			(i & 1) >> 0,
			(i & 2) >> 1,
			(i & 4) >> 2
	) * distance + offset;
	
	return pos;
}