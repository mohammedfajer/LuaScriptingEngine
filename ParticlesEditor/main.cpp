#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"


#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/Graphics/Color.hpp>
#include "imgui_internal.h"

// Particles

namespace particles {

	// MATHEMATICS	- HELPER FUNCTIONS

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD M_PI / 180.0f
#endif

	inline float randomFloat(float low, float high) {
		return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
	}

	inline int randomInt(int low, int high) {
		return (rand() % static_cast<int>(high - low + 1)) + low;
	}

	inline sf::Color randomColor(const sf::Color &low, const sf::Color &high) {
		sf::Uint8 r, g, b, a;

		if (high.r <= low.r) {
			r = high.r;
		}
		else {
			r = (rand() % static_cast<int>(high.r - low.r + 1)) + low.r;
		}

		if (high.g <= low.g) {
			g = high.g;
		}
		else {
			g = (rand() % static_cast<int>(high.g - low.g + 1)) + low.g;
		}

		if (high.b <= low.b) {
			b = high.b;
		}
		else {
			b = (rand() % static_cast<int>(high.b - low.b + 1)) + low.b;
		}

		if (high.a <= low.a) {
			a = high.a;
		}
		else {
			a = (rand() % static_cast<int>(high.a - low.a + 1)) + low.a;
		}

		return { r, g, b, a };
	}

	inline sf::Vector2f randomVector2f(const sf::Vector2f &low, const sf::Vector2f &high) {
		float y = low.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.y - low.y)));
		float x = low.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high.x - low.x)));

		return { x, y };
	}

	inline float dot(const sf::Vector2f &a, const sf::Vector2f &b) {
		return a.x * b.x + a.y * b.y;
	}

	inline float lerpFloat(float a, float b, float alpha) {
		return a * (1.0f - alpha) + b * alpha;
	}

	inline sf::Color lerpColor(const sf::Color &c1, const sf::Color &c2, float alpha) {
		sf::Uint8 r = (sf::Uint8)(c1.r * (1.0f - alpha) + c2.r * alpha);
		sf::Uint8 g = (sf::Uint8)(c1.g * (1.0f - alpha) + c2.g * alpha);
		sf::Uint8 b = (sf::Uint8)(c1.b * (1.0f - alpha) + c2.b * alpha);
		sf::Uint8 a = (sf::Uint8)(c1.a * (1.0f - alpha) + c2.a * alpha);

		return sf::Color(r, g, b, a);
	}


	class ParticleData {
	public:
		explicit ParticleData(int maxCount);
		~ParticleData();

		ParticleData(const ParticleData &) = delete;
		ParticleData &operator=(const ParticleData &) = delete;

		void kill(int id);
		void swapData(int id1, int id2);

	public:
		sf::Vector2f *pos;        // Current position
		sf::Vector2f *vel;        // Current velocity
		sf::Vector2f *acc;        // Current acceleration
		sf::Vector3f *time;       // x: remaining time to live,   y: time to live,    z: interpolation value in [0, 1] of lifetime
		sf::Vector3f *size;       // x: current size,             y: start size,      z: end size
		sf::Vector3f *angle;	  // x: current angle,            y: start rotation,  z: end rotation
		sf::Color *col;        // Current color
		sf::Color *startCol;   // Start color
		sf::Color *endCol;     // End color
		sf::IntRect *texCoords;  // Texture coordinates inside spritesheet
		int *frame;	  // Frame index for animation
		float *frameTimer; // Accumulator for animation

		int           count;
		int           countAlive;
	};



	ParticleData::ParticleData(int maxSize) : count(maxSize), countAlive(0) {
		pos = new sf::Vector2f[maxSize];
		vel = new sf::Vector2f[maxSize];
		acc = new sf::Vector2f[maxSize];
		time = new sf::Vector3f[maxSize];
		size = new sf::Vector3f[maxSize];
		angle = new sf::Vector3f[maxSize];
		col = new sf::Color[maxSize];
		startCol = new sf::Color[maxSize];
		endCol = new sf::Color[maxSize];
		texCoords = new sf::IntRect[maxSize];
		frame = new int[maxSize];
		frameTimer = new float[maxSize];
	}

	ParticleData::~ParticleData() {
		delete pos;
		delete vel;
		delete acc;
		delete time;
		delete size;
		delete angle;
		delete col;
		delete startCol;
		delete endCol;
		delete texCoords;
		delete frame;
		delete frameTimer;
	}

	void ParticleData::kill(int id) {
		if (countAlive > 0) {
			swapData(id, countAlive - 1);
			countAlive--;
		}
	}

	void ParticleData::swapData(int id1, int id2) {
		std::swap(pos[id1], pos[id2]);
		std::swap(vel[id1], vel[id2]);
		std::swap(acc[id1], acc[id2]);
		std::swap(time[id1], time[id2]);
		std::swap(size[id1], size[id2]);
		std::swap(angle[id1], angle[id2]);
		std::swap(col[id1], col[id2]);
		std::swap(startCol[id1], startCol[id2]);
		std::swap(endCol[id1], endCol[id2]);
		std::swap(texCoords[id1], texCoords[id2]);
		std::swap(frame[id1], frame[id2]);
		std::swap(frameTimer[id1], frameTimer[id2]);
	}



	/* Abstract base class for all generators */
	class ParticleGenerator {
	public:
		ParticleGenerator() {}
		virtual ~ParticleGenerator() {}

		virtual void generate(ParticleData *data, int startId, int endId) = 0;
	};


	/* Size Generators */

	class SizeGenerator : public ParticleGenerator {
	public:
		SizeGenerator() {}
		~SizeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minStartSize{ 1.0f };
		float maxStartSize{ 1.0f };
		float minEndSize{ 1.0f };
		float maxEndSize{ 1.0f };
	};

	class ConstantSizeGenerator : public ParticleGenerator {
	public:
		ConstantSizeGenerator() {}
		~ConstantSizeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float size{ 1.0f };
	};


	/* Rotation Generators */

	class RotationGenerator : public ParticleGenerator {
	public:
		RotationGenerator() {}
		~RotationGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minStartAngle{ 0.0f };
		float maxStartAngle{ 0.0f };
		float minEndAngle{ 0.0f };
		float maxEndAngle{ 0.0f };
	};

	class ConstantRotationGenerator : public ParticleGenerator {
	public:
		ConstantRotationGenerator() {}
		~ConstantRotationGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float angle{ 0.0f };
	};

	class DirectionDefinedRotationGenerator : public ParticleGenerator {
	public:
		DirectionDefinedRotationGenerator() {}
		~DirectionDefinedRotationGenerator() {}

		void generate(ParticleData *data, int startId, int endId);
	};


	/* Color Generators */

	class ColorGenerator : public ParticleGenerator {
	public:
		ColorGenerator() {}
		~ColorGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Color minStartCol{ sf::Color::Black };
		sf::Color maxStartCol{ sf::Color::Black };
		sf::Color minEndCol{ sf::Color::Black };
		sf::Color maxEndCol{ sf::Color::Black };
	};

	class ConstantColorGenerator : public ParticleGenerator {
	public:
		ConstantColorGenerator() {}
		~ConstantColorGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Color color{ sf::Color::Black };
	};


	/* Velocity Generators */

	class VectorVelocityGenerator : public ParticleGenerator {
	public:
		VectorVelocityGenerator() {}
		~VectorVelocityGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f minStartVel{ 0.0f, 0.0f };
		sf::Vector2f maxStartVel{ 0.0f, 0.0f };
	};

	class AngledVelocityGenerator : public ParticleGenerator {
	public:
		AngledVelocityGenerator() {}
		~AngledVelocityGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minAngle{ 0.0f };
		float maxAngle{ 0.0f };
		float minStartSpeed{ 0.0f };
		float maxStartSpeed{ 0.0f };
	};

	class AimedVelocityGenerator : public ParticleGenerator {
	public:
		AimedVelocityGenerator() {}
		~AimedVelocityGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f goal{ 0.f, 0.f };
		float minStartSpeed{ 0.0f };
		float maxStartSpeed{ 0.0f };
	};


	/* Time Generators */

	class TimeGenerator : public ParticleGenerator {
	public:
		TimeGenerator() {}
		~TimeGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		float minTime{ 0.0f };
		float maxTime{ 0.0f };
	};


	/* Texture Coordinates Generators */

	class TexCoordsGenerator : public ParticleGenerator {
	public:
		TexCoordsGenerator() {}
		~TexCoordsGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		sf::IntRect texCoords{ 0, 0, 1, 1 };
	};

	class TexCoordsRandomGenerator : public ParticleGenerator {
	public:
		TexCoordsRandomGenerator() {}
		~TexCoordsRandomGenerator() {}

		void generate(ParticleData *data, int startId, int endId);

	public:
		std::vector<sf::IntRect> texCoords;
	};

	/* Size Generators */

	void SizeGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float startSize = randomFloat(minStartSize, maxStartSize);
			float endSize = randomFloat(minEndSize, maxEndSize);
			data->size[i].x = data->size[i].y = startSize;
			data->size[i].z = endSize;
		}
	}

	void ConstantSizeGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->size[i].x = data->size[i].y = data->size[i].z = size;
		}
	}


	/* Rotation Generators */

	void RotationGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float startPhi = DEG_TO_RAD * (randomFloat(minStartAngle, maxStartAngle));
			float endPhi = DEG_TO_RAD * (randomFloat(minEndAngle, maxEndAngle));
			data->angle[i].x = data->angle[i].y = startPhi;
			data->angle[i].z = endPhi;
		}
	}

	void ConstantRotationGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = DEG_TO_RAD * (angle);
			data->angle[i].x = data->angle[i].y = data->angle[i].z = phi;
		}
	}

	void DirectionDefinedRotationGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = 0.5f * M_PI - std::atan2(-data->vel[i].y, data->vel[i].x);
			data->angle[i].x = data->angle[i].y = data->angle[i].z = phi;
		}
	}


	/* Color Generators */

	void ColorGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->startCol[i] = randomColor(minStartCol, maxStartCol);
			data->endCol[i] = randomColor(minEndCol, maxEndCol);
		}
	}

	void ConstantColorGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->startCol[i] = data->endCol[i] = color;
		}
	}


	/* Velocity Generators */

	void VectorVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->vel[i] = randomVector2f(minStartVel, maxStartVel);
		}
	}

	void AngledVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = DEG_TO_RAD * (randomFloat(minAngle, maxAngle) - 90.0f);		// offset to start at top instead of "mathematical 0 degrees"
			sf::Vector2f dir{ std::cos(phi), std::sin(phi) };
			float len = randomFloat(minStartSpeed, maxStartSpeed);
			data->vel[i] = dir * len;
		}
	}

	void AimedVelocityGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			sf::Vector2f dir = goal - data->pos[i];
			float magnitude = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			dir /= magnitude;
			float len = randomFloat(minStartSpeed, maxStartSpeed);
			data->vel[i] = dir * len;
		}
	}


	/* Time Generators */

	void TimeGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->time[i].x = data->time[i].y = randomFloat(minTime, maxTime);
			data->time[i].z = 0.0f;
		}
	}


	/* Texture Coordinates Generators */

	void TexCoordsGenerator::generate(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->texCoords[i] = texCoords;
			data->frame[i] = 0;
			data->frameTimer[i] = 0.f;
		}
	}

	void TexCoordsRandomGenerator::generate(ParticleData *data, int startId, int endId) {
		int low = 0;
		int high = static_cast<int>(texCoords.size() - 1);
		if (high < low) return;
		for (int i = startId; i < endId; ++i) {
			int idx = randomInt(low, high);
			data->texCoords[i] = texCoords[idx];
			data->frame[i] = idx;
			data->frameTimer[i] = 0.f;
		}
	}

	/* Abstract base class for all generators */
	class ParticleSpawner {
	public:
		ParticleSpawner() {}
		virtual ~ParticleSpawner() {}

		virtual void spawn(ParticleData *data, int startId, int endId) = 0;

	public:
		sf::Vector2f center{ 0.0f, 0.0f };
	};


	class PointSpawner : public ParticleSpawner {
	public:
		PointSpawner() {}
		~PointSpawner() {}

		void spawn(ParticleData *data, int startId, int endId);
	};

	class BoxSpawner : public ParticleSpawner {
	public:
		BoxSpawner() {}
		~BoxSpawner() {}

		void spawn(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f size{ 0.0f, 0.0f };
	};

	class CircleSpawner : public ParticleSpawner {
	public:
		CircleSpawner() {}
		~CircleSpawner() {}

		void spawn(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f radius{ 0.0f, 0.0f };
	};

	class DiskSpawner : public ParticleSpawner {
	public:
		DiskSpawner() {}
		~DiskSpawner() {}

		void spawn(ParticleData *data, int startId, int endId);

	public:
		sf::Vector2f radius{ 0.0f, 0.0f };
	};

	void PointSpawner::spawn(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			data->pos[i] = center;
		}
	}

	void BoxSpawner::spawn(ParticleData *data, int startId, int endId) {
		float sx = 0.5f * size.x;
		float sy = 0.5f * size.y;
		sf::Vector2f posMin{ center.x - sx, center.y - sy };
		sf::Vector2f posMax{ center.x + sx, center.y + sy };

		for (int i = startId; i < endId; ++i) {
			data->pos[i] = randomVector2f(posMin, posMax);
		}
	}

	void CircleSpawner::spawn(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			data->pos[i] = { center.x + radius.x * std::cos(phi),
							 center.y + radius.y * std::sin(phi) };
		}
	}

	void DiskSpawner::spawn(ParticleData *data, int startId, int endId) {
		for (int i = startId; i < endId; ++i) {
			float phi = randomFloat(0.0f, M_PI * 2.0f);
			float jacobian = std::sqrt(randomFloat(0.0f, 1.f));
			data->pos[i] = { center.x + jacobian * radius.x * std::cos(phi),
							 center.y + jacobian * radius.y * std::sin(phi) };
		}
	}

	/* Abstract base class for all particle updators */
	class ParticleUpdater {
	public:
		ParticleUpdater() {}
		virtual ~ParticleUpdater() {}

		virtual void update(ParticleData *data, float dt) = 0;
	};


	class EulerUpdater : public ParticleUpdater {
	public:
		EulerUpdater() {}
		~EulerUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		sf::Vector2f globalAcceleration{ 0.0f, 0.0f };
	};


	class HorizontalCollisionUpdater : public ParticleUpdater {
	public:
		HorizontalCollisionUpdater() {}
		~HorizontalCollisionUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		float pos{ 0.0f };
		float bounceFactor{ 0.5f };
	};


	class VerticalCollisionUpdater : public ParticleUpdater {
	public:
		VerticalCollisionUpdater() {}
		~VerticalCollisionUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		float pos{ 0.0f };
		float bounceFactor{ 0.5f };
	};


	class AttractorUpdater : public ParticleUpdater {
	public:
		AttractorUpdater() {}
		~AttractorUpdater() {}

		void update(ParticleData *data, float dt);

		size_t numAttractors() const { return m_attractors.size(); }
		void add(const sf::Vector3f &attr) { m_attractors.push_back(attr); }
		sf::Vector3f &get(int id) { return m_attractors[id]; }

	protected:
		std::vector<sf::Vector3f> m_attractors;	// .xy is position, .z is force
	};


	class SizeUpdater : public ParticleUpdater {
	public:
		SizeUpdater() {}
		~SizeUpdater() {}

		void update(ParticleData *data, float dt);
	};


	class RotationUpdater : public ParticleUpdater {
	public:
		RotationUpdater() {}
		~RotationUpdater() {}

		void update(ParticleData *data, float dt);
	};


	class ColorUpdater : public ParticleUpdater {
	public:
		ColorUpdater() {}
		~ColorUpdater() {}

		void update(ParticleData *data, float dt);
	};


	class TimeUpdater : public ParticleUpdater {
	public:
		TimeUpdater() {}
		~TimeUpdater() {}

		void update(ParticleData *data, float dt);
	};


	class AnimationUpdater : public ParticleUpdater {
	public:
		AnimationUpdater() {}
		~AnimationUpdater() {}

		void update(ParticleData *data, float dt);

	public:
		std::vector<sf::IntRect> frames;
		float frameTime;
		bool looped{ false };
	};

	void EulerUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			data->acc[i] += globalAcceleration;
		}

		for (int i = 0; i < endId; ++i) {
			data->pos[i] += dt * data->vel[i];
		}

		for (int i = 0; i < endId; ++i) {
			data->vel[i] += dt * data->acc[i];
		}
	}


	void HorizontalCollisionUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float x = data->pos[i].x;
			float xPrime = x + dt * data->vel[i].x;

			if ((x < pos && xPrime >= pos) || (x > pos && xPrime <= pos)) {
				data->pos[i].x = pos;

				sf::Vector2f acc = data->acc[i];
				data->acc[i] = sf::Vector2f(-acc.x * bounceFactor, acc.y);

				sf::Vector2f vel = data->vel[i];
				data->vel[i] = sf::Vector2f(-vel.x * bounceFactor, vel.y);
			}
		}
	}


	void VerticalCollisionUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float y = data->pos[i].y;
			float yPrime = y + dt * data->vel[i].y;

			if ((y < pos && yPrime >= pos) || (y > pos && yPrime <= pos)) {
				data->pos[i].y = pos;

				sf::Vector2f acc = data->acc[i];
				data->acc[i] = sf::Vector2f(acc.x, -acc.y * bounceFactor);

				sf::Vector2f vel = data->vel[i];
				data->vel[i] = sf::Vector2f(vel.x, -vel.y * bounceFactor);
			}
		}
	}


	void AttractorUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;
		int numAttractors = static_cast<int>(m_attractors.size());
		sf::Vector2f off;
		float dist;

		for (int i = 0; i < endId; ++i) {
			for (int j = 0; j < numAttractors; ++j) {
				off.x = m_attractors[j].x - data->pos[i].x;
				off.y = m_attractors[j].y - data->pos[i].y;
				dist = dot(off, off);
				dist = m_attractors[j].z / dist;

				data->acc[i] += off * dist;
			}
		}
	}


	void SizeUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			data->size[i].x = lerpFloat(data->size[i].y, data->size[i].z, a);
		}
	}


	void RotationUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			data->angle[i].x = lerpFloat(data->angle[i].y, data->angle[i].z, a);
		}
	}


	void ColorUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;

		for (int i = 0; i < endId; ++i) {
			float a = data->time[i].z;
			data->col[i] = lerpColor(data->startCol[i], data->endCol[i], a);
		}
	}


	void TimeUpdater::update(ParticleData *data, float dt) {
		int endId = data->countAlive;

		if (endId == 0) return;

		for (int i = 0; i < endId; ++i) {
			data->time[i].x -= dt;
			data->time[i].z = 1.0f - (data->time[i].x / data->time[i].y);

			if (data->time[i].x < 0.0f) {
				data->kill(i);
				endId = data->countAlive;
			}
		}
	}


	void AnimationUpdater::update(ParticleData *data, float dt) {
		const int endId = data->countAlive;
		int animationSize = static_cast<int>(frames.size());

		for (int i = 0; i < endId; ++i) {
			float currentTime = data->frameTimer[i];
			currentTime += dt;

			if (currentTime >= frameTime) {
				currentTime = std::fmod(currentTime, frameTime);

				int frame = data->frame[i];
				if (frame + 1 < animationSize) {
					frame++;
				}
				else if (looped) {
					frame = 0;
				}
				data->frame[i] = frame;
				data->texCoords[i] = frames[frame];
			}

			data->frameTimer[i] = currentTime;
		}
	}


	/* Abstract base class for all particle system types */
	class ParticleSystem : public sf::Transformable {
	public:
		ParticleSystem(int maxCount);
		virtual ~ParticleSystem();

		ParticleSystem(const ParticleSystem &) = delete;
		ParticleSystem &operator=(const ParticleSystem &) = delete;

		void reset();

		virtual void update(const sf::Time &dt);
		virtual void render(sf::RenderTarget &renderTarget) = 0;

		template<typename T>
		inline T *addGenerator() {
			T *g = new T();
			m_generators.push_back(g);
			return g;
		}

		template<typename T>
		inline T *addSpawner() {
			T *s = new T();
			m_spawners.push_back(s);
			return s;
		}

		template<typename T>
		inline T *addUpdater() {
			T *u = new T();
			m_updaters.push_back(u);
			return u;
		}

		void removeGenerator(ParticleGenerator *g);
		void removeSpawner(ParticleSpawner *s);
		void removeUpdater(ParticleUpdater *u);

		void emitParticles(int count); 	// emit a fix number of particles

		inline size_t getNumberGenerators() const { return m_generators.size(); }
		inline size_t getNumberSpawners() const { return m_spawners.size(); }
		inline size_t getNumberUpdaters() const { return m_updaters.size(); }

		inline void clearGenerators() {
			for (auto g : m_generators) {
				delete g;
			}
			m_generators.clear();
		}

		inline void clearSpawners() {
			for (auto s : m_spawners) {
				delete s;
			}
			m_spawners.clear();
		}

		inline void clearUpdaters() {
			for (auto u : m_updaters) {
				delete u;
			}
			m_updaters.clear();
		}

	protected:
		void emitWithRate(float dt);	// emit a stream of particles defined by emitRate and dt

	public:
		float	emitRate;	// Note: For a constant particle stream, it should hold that: emitRate <= (maximalParticleCount / averageParticleLifetime)

	protected:
		float m_dt;

		ParticleData *m_particles;

		std::vector<ParticleGenerator *> m_generators;
		std::vector<ParticleSpawner *> m_spawners;
		std::vector<ParticleUpdater *> m_updaters;

		sf::VertexArray m_vertices;
	};


	class PointParticleSystem : public ParticleSystem {
	public:
		PointParticleSystem(int maxCount);
		virtual ~PointParticleSystem() {}

		PointParticleSystem(const PointParticleSystem &) = delete;
		PointParticleSystem &operator=(const PointParticleSystem &) = delete;

		virtual void render(sf::RenderTarget &renderTarget) override;

	protected:
		void updateVertices();
	};


	class TextureParticleSystem : public ParticleSystem {
	public:
		TextureParticleSystem(int maxCount, sf::Texture *texture);
		virtual ~TextureParticleSystem() {}

		TextureParticleSystem(const TextureParticleSystem &) = delete;
		TextureParticleSystem &operator=(const TextureParticleSystem &) = delete;

		virtual void render(sf::RenderTarget &renderTarget) override;

		void setTexture(sf::Texture *texture);

	protected:
		void updateVertices();

	public:
		bool additiveBlendMode;

	protected:
		sf::Texture *m_texture;
	};


	class SpriteSheetParticleSystem : public TextureParticleSystem {
	public:
		SpriteSheetParticleSystem(int maxCount, sf::Texture *texture) : TextureParticleSystem(maxCount, texture) {}
		virtual ~SpriteSheetParticleSystem() {}

		SpriteSheetParticleSystem(const SpriteSheetParticleSystem &) = delete;
		SpriteSheetParticleSystem &operator=(const SpriteSheetParticleSystem &) = delete;

		virtual void render(sf::RenderTarget &renderTarget) override;

	protected:
		void updateVertices();
	};


	class MetaballParticleSystem : public TextureParticleSystem {
	public:
		MetaballParticleSystem(int maxCount, sf::Texture *texture, int windowWidth, int windowHeight);
		virtual ~MetaballParticleSystem() {}

		MetaballParticleSystem(const MetaballParticleSystem &) = delete;
		MetaballParticleSystem &operator=(const MetaballParticleSystem &) = delete;

		virtual void render(sf::RenderTarget &renderTarget) override;

	public:
		sf::Color color{ sf::Color::White };
		float threshold{ 0.5f };

	protected:
		sf::RenderTexture m_renderTexture;
		sf::Sprite m_sprite;
		sf::Shader m_shader;
	};


	/* ParticleSystem */

	ParticleSystem::ParticleSystem(int maxCount) : emitRate(0.f), m_dt(0.f) {
		m_particles = new ParticleData(maxCount);
	}

	ParticleSystem::~ParticleSystem() {
		delete m_particles;

		for (auto s : m_spawners) {
			delete s;
		}

		for (auto g : m_generators) {
			delete g;
		}

		for (auto u : m_updaters) {
			delete u;
		}
	}

	void ParticleSystem::removeGenerator(ParticleGenerator *g) {
		if (g == nullptr) return;
		auto it = std::find(m_generators.begin(), m_generators.end(), g);
		if (it == m_generators.end()) return;
		m_generators.erase(it);
		delete g;
	}

	void ParticleSystem::removeSpawner(ParticleSpawner *s) {
		if (s == nullptr) return;
		auto it = std::find(m_spawners.begin(), m_spawners.end(), s);
		if (it == m_spawners.end()) return;
		m_spawners.erase(it);
		delete s;
	}

	void ParticleSystem::removeUpdater(ParticleUpdater *u) {
		if (u == nullptr) return;
		auto it = std::find(m_updaters.begin(), m_updaters.end(), u);
		if (it == m_updaters.end()) return;
		m_updaters.erase(it);
		delete u;
	}

	void ParticleSystem::emitWithRate(float dt) {
		m_dt += dt;

		int maxNewParticles = 0;

		if (m_dt * emitRate > 1.0f) {
			maxNewParticles = static_cast<int>(m_dt * emitRate);
			m_dt -= maxNewParticles / emitRate;
		}

		if (maxNewParticles == 0) return;

		emitParticles(maxNewParticles);
	}

	void ParticleSystem::emitParticles(int count) {
		if (m_spawners.size() == 0) return;

		const int startId = m_particles->countAlive;
		const int endId = std::min(startId + count, m_particles->count - 1);
		const int newParticles = endId - startId;

		const int nSpawners = static_cast<int>(m_spawners.size());
		const int spawnerCount = newParticles / nSpawners;
		const int remainder = newParticles - spawnerCount * nSpawners;
		int spawnerStartId = startId;
		for (int i = 0; i < nSpawners; ++i) {
			int numberToSpawn = (i < remainder) ? spawnerCount + 1 : spawnerCount;
			m_spawners[i]->spawn(m_particles, spawnerStartId, spawnerStartId + numberToSpawn);
			spawnerStartId += numberToSpawn;
		}

		for (auto &generator : m_generators) {
			generator->generate(m_particles, startId, endId);
		}

		m_particles->countAlive += newParticles;
	}

	void ParticleSystem::update(const sf::Time &dt) {
		if (emitRate > 0.0f) {
			emitWithRate(dt.asSeconds());
		}

		for (int i = 0; i < m_particles->countAlive; ++i) {
			m_particles->acc[i] = { 0.0f, 0.0f };
		}

		for (auto &updater : m_updaters) {
			updater->update(m_particles, dt.asSeconds());
		}
	}

	void ParticleSystem::reset() {
		m_particles->countAlive = 0;
	}


	/* PointParticleSystem */

	PointParticleSystem::PointParticleSystem(int maxCount) : ParticleSystem(maxCount) {
		m_vertices = sf::VertexArray(sf::Points, maxCount);
	}

	void PointParticleSystem::render(sf::RenderTarget &renderTarget) {
		updateVertices();

		if (m_particles->countAlive <= 0) return;

		sf::RenderStates states = sf::RenderStates::Default;

		const sf::Vertex *ver = &m_vertices[0];
		renderTarget.draw(ver, m_particles->countAlive, sf::Points, states);
	}

	void PointParticleSystem::updateVertices() {
		for (int i = 0; i < m_particles->countAlive; ++i) {
			m_vertices[i].position = m_particles->pos[i];
			m_vertices[i].color = m_particles->col[i];
		}
	}


	/* TextureParticleSystem */

	TextureParticleSystem::TextureParticleSystem(int maxCount, sf::Texture *texture) : ParticleSystem(maxCount), m_texture(texture) {
		m_vertices = sf::VertexArray(sf::Quads, maxCount * 4);

		float x = static_cast<float>(m_texture->getSize().x);
		float y = static_cast<float>(m_texture->getSize().y);

		for (int i = 0; i < m_particles->count; ++i) {
			m_vertices[4 * i + 0].texCoords = sf::Vector2f(0.f, 0.f);
			m_vertices[4 * i + 1].texCoords = sf::Vector2f(x, 0.f);
			m_vertices[4 * i + 2].texCoords = sf::Vector2f(x, y);
			m_vertices[4 * i + 3].texCoords = sf::Vector2f(0.f, y);

			m_vertices[4 * i + 0].color = sf::Color::White;
			m_vertices[4 * i + 1].color = sf::Color::White;
			m_vertices[4 * i + 2].color = sf::Color::White;
			m_vertices[4 * i + 3].color = sf::Color::White;
		}

		additiveBlendMode = false;
	}

	void TextureParticleSystem::setTexture(sf::Texture *texture) {
		m_texture = texture;

		float x = static_cast<float>(m_texture->getSize().x);
		float y = static_cast<float>(m_texture->getSize().y);

		for (int i = 0; i < m_particles->count; ++i) {
			m_vertices[4 * i + 0].texCoords = sf::Vector2f(0.f, 0.f);
			m_vertices[4 * i + 1].texCoords = sf::Vector2f(x, 0.f);
			m_vertices[4 * i + 2].texCoords = sf::Vector2f(x, y);
			m_vertices[4 * i + 3].texCoords = sf::Vector2f(0.f, y);
		}
	}

	void TextureParticleSystem::updateVertices() {
		for (int i = 0; i < m_particles->countAlive; ++i) {
			float size = 0.5f * m_particles->size[i].x;
			float angle = m_particles->angle[i].x;

			m_vertices[4 * i + 0].position.x = -size;	m_vertices[4 * i + 0].position.y = -size;
			m_vertices[4 * i + 1].position.x = +size;	m_vertices[4 * i + 1].position.y = -size;
			m_vertices[4 * i + 2].position.x = +size;	m_vertices[4 * i + 2].position.y = +size;
			m_vertices[4 * i + 3].position.x = -size;	m_vertices[4 * i + 3].position.y = +size;

			if (angle != 0.f) {
				float sin = std::sin(angle); float cos = std::cos(angle);

				for (int j = 0; j < 4; ++j) {
					float x = m_vertices[4 * i + j].position.x;
					float y = m_vertices[4 * i + j].position.y;

					m_vertices[4 * i + j].position.x = cos * x - sin * y;
					m_vertices[4 * i + j].position.y = sin * x + cos * y;
				}
			}

			m_vertices[4 * i + 0].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 0].position.y += m_particles->pos[i].y;
			m_vertices[4 * i + 1].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 1].position.y += m_particles->pos[i].y;
			m_vertices[4 * i + 2].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 2].position.y += m_particles->pos[i].y;
			m_vertices[4 * i + 3].position.x += m_particles->pos[i].x;	m_vertices[4 * i + 3].position.y += m_particles->pos[i].y;

			m_vertices[4 * i + 0].color = m_particles->col[i];
			m_vertices[4 * i + 1].color = m_particles->col[i];
			m_vertices[4 * i + 2].color = m_particles->col[i];
			m_vertices[4 * i + 3].color = m_particles->col[i];
		}
	}

	void TextureParticleSystem::render(sf::RenderTarget &renderTarget) {
		updateVertices();

		if (m_particles->countAlive <= 0) return;

		sf::RenderStates states = sf::RenderStates::Default;

		if (additiveBlendMode) {
			states.blendMode = sf::BlendAdd;
		}

		states.texture = m_texture;

		const sf::Vertex *ver = &m_vertices[0];
		renderTarget.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
	}


	/* SpriteSheetParticleSystem */

	void SpriteSheetParticleSystem::render(sf::RenderTarget &renderTarget) {
		updateVertices();

		if (m_particles->countAlive <= 0) return;

		sf::RenderStates states = sf::RenderStates::Default;

		if (additiveBlendMode) {
			states.blendMode = sf::BlendAdd;
		}

		states.texture = m_texture;

		const sf::Vertex *ver = &m_vertices[0];
		renderTarget.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
	}

	void SpriteSheetParticleSystem::updateVertices() {
		TextureParticleSystem::updateVertices();

		for (int i = 0; i < m_particles->countAlive; ++i) {
			float left = static_cast<float>(m_particles->texCoords[i].left);
			float top = static_cast<float>(m_particles->texCoords[i].top);
			float width = static_cast<float>(m_particles->texCoords[i].width);
			float height = static_cast<float>(m_particles->texCoords[i].height);

			m_vertices[4 * i + 0].texCoords = sf::Vector2f(left, top);
			m_vertices[4 * i + 1].texCoords = sf::Vector2f(left + width, top);
			m_vertices[4 * i + 2].texCoords = sf::Vector2f(left + width, top + height);
			m_vertices[4 * i + 3].texCoords = sf::Vector2f(left, top + height);
		}
	}


	/* MetaballParticleSystem */

	const std::string vertexShader = R"(
		void main()
		{
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
			gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
			gl_FrontColor = gl_Color;
		}
	)";

	const std::string fragmentShader = R"(
		uniform sampler2D texture;
		uniform vec4 customColor;
		uniform float threshold;

		void main() {
			vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
			if (pixel.a > threshold) {
				gl_FragColor = customColor;
			} 
			else {
				gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
			}
		}
	)";

	MetaballParticleSystem::MetaballParticleSystem(int maxCount, sf::Texture *texture, int windowWidth, int windowHeight) : TextureParticleSystem(maxCount, texture) {
		additiveBlendMode = true;
		m_shader.setUniform("texture", sf::Shader::CurrentTexture);
		m_shader.loadFromMemory(vertexShader, fragmentShader);
		m_renderTexture.create(windowWidth, windowHeight);
	}

	void MetaballParticleSystem::render(sf::RenderTarget &renderTarget) {
		updateVertices();

		if (m_particles->countAlive <= 0) return;

		sf::RenderStates states = sf::RenderStates::Default;
		states.blendMode = sf::BlendAdd;

		states.texture = m_texture;

		const sf::Vertex *ver = &m_vertices[0];

		sf::View oldView = renderTarget.getView();
		sf::View defaultView = renderTarget.getDefaultView();

		m_renderTexture.setView(oldView);
		m_renderTexture.clear(sf::Color(0, 0, 0, 0));
		m_renderTexture.draw(ver, m_particles->countAlive * 4, sf::Quads, states);
		m_renderTexture.display();
		m_sprite.setTexture(m_renderTexture.getTexture());
		sf::Glsl::Vec4 colorVec = sf::Glsl::Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
		m_shader.setUniform("customColor", colorVec);
		m_shader.setUniform("threshold", threshold);

		renderTarget.setView(defaultView);
		renderTarget.draw(m_sprite, &m_shader);
		renderTarget.setView(oldView);
	}
}


namespace ImGui
{
	bool ColorEdit(const char *label, sf::Color *col, bool show_alpha = true);
	bool SliderFloat2(const char *label, sf::Vector2f *vec, float v_min, float v_max, const char *display_format = "%.3f", float power = 1.0f);

	bool ColorEdit(const char *label, sf::Color *col, bool show_alpha) {
		float col4[4];
		col4[0] = (float)col->r / 255.f;
		col4[1] = (float)col->g / 255.f;
		col4[2] = (float)col->b / 255.f;
		col4[3] = (float)col->a / 255.f;
		const bool value_changed = ColorEdit4(label, col4, show_alpha);
		col->r = (sf::Uint8)(col4[0] * 255.f);
		col->g = (sf::Uint8)(col4[1] * 255.f);
		col->b = (sf::Uint8)(col4[2] * 255.f);
		col->a = (sf::Uint8)(col4[3] * 255.f);
		return value_changed;
	}

	bool SliderFloat2(const char *label, sf::Vector2f *vec,
		float v_min, float v_max, const char *display_format, float power) {
		bool value_changed_x = ImGui::SliderFloat((std::string(label) + " X").c_str(), &vec->x, v_min, v_max, display_format, power);
		bool value_changed_y = ImGui::SliderFloat((std::string(label) + " Y").c_str(), &vec->y, v_min, v_max, display_format, power);
		return value_changed_x || value_changed_y;
	}

}



const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int MAX_PARTICLE_COUNT = 100000;

enum ParticleSystemMode {
	Points,
	Texture,
	Spritesheet,
	AnimatedSpritesheet,
	Metaball
};

enum SpawnerMode {
	Point,
	Box,
	Circle,
	Disk
};

enum VelocityGeneratorMode {
	Angled,
	Vector,
	Aimed
};

void initParticleSystem();
void setSpawnMode();
void setVelocityGeneratorMode();
void gui();

// Particle System
particles::ParticleSystem *particleSystem;

// Particle Spawner
particles::ParticleSpawner *spawner;

// Particle Generators
particles::ParticleGenerator *velocityGenerator;
particles::TimeGenerator *timeGenerator;
particles::RotationGenerator *rotationGenerator;
particles::ColorGenerator *colorGenerator;
particles::SizeGenerator *sizeGenerator;

// Particle Updaters
particles::TimeUpdater *timeUpdater;
particles::RotationUpdater *rotationUpdater;
particles::ColorUpdater *colorUpdater;
particles::SizeUpdater *sizeUpdater;
particles::EulerUpdater *eulerUpdater;

// Global State
ParticleSystemMode particleSystemMode = ParticleSystemMode::Texture;
SpawnerMode spawnerMode = SpawnerMode::Point;
VelocityGeneratorMode velocityGeneratorMode = VelocityGeneratorMode::Angled;

sf::Texture *circleTexture;
sf::Texture *blobTexture;
sf::Texture *starTexture;
sf::Texture *spritesheetTexture;

void initParticleSystem() {
	if (particleSystem) delete particleSystem;

	switch (particleSystemMode) {
	case Points:
		particleSystem = new particles::PointParticleSystem(MAX_PARTICLE_COUNT);
		break;
	case Spritesheet:
	case AnimatedSpritesheet:
		particleSystem = new particles::SpriteSheetParticleSystem(MAX_PARTICLE_COUNT, spritesheetTexture);
		break;
	case Metaball:
		particleSystem = new particles::MetaballParticleSystem(MAX_PARTICLE_COUNT, blobTexture, WINDOW_WIDTH, WINDOW_HEIGHT);
		break;
	case Texture:
	default:
		particleSystem = new particles::TextureParticleSystem(MAX_PARTICLE_COUNT, circleTexture);
		break;
	}

	particleSystem->emitRate = 160.f;

	setSpawnMode();
	setVelocityGeneratorMode();

	timeGenerator = particleSystem->addGenerator<particles::TimeGenerator>();
	timeGenerator->minTime = 1.f;
	timeGenerator->maxTime = 5.f;

	colorGenerator = particleSystem->addGenerator<particles::ColorGenerator>();
	colorGenerator->minStartCol = sf::Color(16, 124, 167, 255);
	colorGenerator->maxStartCol = sf::Color(30, 150, 255, 255);
	colorGenerator->minEndCol = sf::Color(57, 0, 150, 0);
	colorGenerator->maxEndCol = sf::Color(235, 128, 220, 0);

	sizeGenerator = particleSystem->addGenerator<particles::SizeGenerator>();
	sizeGenerator->minStartSize = 20.f;
	sizeGenerator->maxStartSize = 60.f;
	sizeGenerator->minEndSize = 10.f;
	sizeGenerator->maxEndSize = 30.f;

	rotationGenerator = particleSystem->addGenerator<particles::RotationGenerator>();
	rotationGenerator->minStartAngle = -20.f;
	rotationGenerator->maxStartAngle = -20.f;
	rotationGenerator->minEndAngle = 90.f;
	rotationGenerator->maxEndAngle = 90.f;

	timeUpdater = particleSystem->addUpdater<particles::TimeUpdater>();
	colorUpdater = particleSystem->addUpdater<particles::ColorUpdater>();
	sizeUpdater = particleSystem->addUpdater<particles::SizeUpdater>();
	rotationUpdater = particleSystem->addUpdater<particles::RotationUpdater>();
	eulerUpdater = particleSystem->addUpdater<particles::EulerUpdater>();

	if (particleSystemMode == ParticleSystemMode::Spritesheet) {
		auto texCoordGen = particleSystem->addGenerator<particles::TexCoordsRandomGenerator>();
		texCoordGen->texCoords.push_back(sf::IntRect(0, 0, 8, 8));
		texCoordGen->texCoords.push_back(sf::IntRect(8, 0, 8, 8));
		texCoordGen->texCoords.push_back(sf::IntRect(16, 0, 8, 8));
		texCoordGen->texCoords.push_back(sf::IntRect(24, 0, 8, 8));
	}
	else if (particleSystemMode == ParticleSystemMode::AnimatedSpritesheet) {
		auto texCoordGen = particleSystem->addGenerator<particles::TexCoordsGenerator>();
		texCoordGen->texCoords = sf::IntRect(0, 0, 8, 8);

		auto animationUpdater = particleSystem->addUpdater<particles::AnimationUpdater>();
		animationUpdater->frames.push_back(sf::IntRect(0, 0, 8, 8));
		animationUpdater->frames.push_back(sf::IntRect(8, 0, 8, 8));
		animationUpdater->frames.push_back(sf::IntRect(16, 0, 8, 8));
		animationUpdater->frames.push_back(sf::IntRect(24, 0, 8, 8));

		animationUpdater->frameTime = 0.8f;
		animationUpdater->looped = true;
	}
	else if (particleSystemMode == ParticleSystemMode::Metaball) {
		auto ps = dynamic_cast<particles::MetaballParticleSystem *>(particleSystem);
		ps->color = sf::Color(20, 50, 100, 255);
	}
}

void setSpawnMode() {
	if (spawner) particleSystem->removeSpawner(spawner);

	switch (spawnerMode) {
	case Box: {
		auto boxSpawner = particleSystem->addSpawner<particles::BoxSpawner>();
		boxSpawner->size = sf::Vector2f(160.f, 160.f);
		spawner = boxSpawner;
	}
			break;

	case Circle: {
		auto circleSpawner = particleSystem->addSpawner<particles::CircleSpawner>();
		circleSpawner->radius = sf::Vector2f(70.f, 40.f);
		spawner = circleSpawner;
	}
			   break;

	case Disk: {
		auto diskSpawner = particleSystem->addSpawner<particles::DiskSpawner>();
		diskSpawner->radius = sf::Vector2f(70.f, 40.f);
		spawner = diskSpawner;
	}
			 break;

	case Point:
	default: {
		spawner = particleSystem->addSpawner<particles::PointSpawner>();
	}
		   break;
	}
}

void setVelocityGeneratorMode() {
	if (velocityGenerator) particleSystem->removeGenerator(velocityGenerator);

	switch (velocityGeneratorMode) {
	case Vector: {
		auto vectorGenerator = particleSystem->addGenerator<particles::VectorVelocityGenerator>();
		vectorGenerator->minStartVel = sf::Vector2f(20.f, -20.f);
		vectorGenerator->maxStartVel = sf::Vector2f(40.f, -40.f);
		velocityGenerator = vectorGenerator;
	}
			   break;

	case Aimed: {
		auto aimedGenerator = particleSystem->addGenerator<particles::AimedVelocityGenerator>();
		aimedGenerator->goal = sf::Vector2f(0.5f * WINDOW_WIDTH, 0.5f * WINDOW_HEIGHT);
		aimedGenerator->minStartSpeed = 100.f;
		aimedGenerator->maxStartSpeed = 150.f;
		velocityGenerator = aimedGenerator;
	}
			  break;

	case Angled:
	default: {
		auto angledGenerator = particleSystem->addGenerator<particles::AngledVelocityGenerator>();
		angledGenerator->minAngle = -20.f;
		angledGenerator->maxAngle = 20.f;
		angledGenerator->minStartSpeed = 100.f;
		angledGenerator->maxStartSpeed = 150.f;
		velocityGenerator = angledGenerator;
	}
		   break;
	}
}

void gui() {
	static bool first_use = true;
	if (first_use) {
		ImGui::SetNextWindowSize(ImVec2(380, 630), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
		first_use = false;
	}
	

	ImGui::SetNextWindowSize(ImVec2(380, 630), /*ImGuiSetCond_FirstUseEver*/ ImGuiCond_Once);

	ImGui::Begin("Particles Demo");

	if (ImGui::CollapsingHeader("Particle System", ImGuiTreeNodeFlags_DefaultOpen)) {
		const char *particleSystemItems[] = { "Points", "Textured", "Spritesheet", "Animated Spritesheet", "Metaball" };
		static int particleSystemItem = 1;
		if (ImGui::Combo("Render Mode", &particleSystemItem, particleSystemItems, 5)) {
			particleSystemMode = static_cast<ParticleSystemMode>(particleSystemItem);
			initParticleSystem();
		}

		ImGui::SliderFloat("emit rate", &particleSystem->emitRate, 0.f, 1500.f);
		if (particleSystemMode == ParticleSystemMode::Texture || particleSystemMode == ParticleSystemMode::Spritesheet || particleSystemMode == ParticleSystemMode::AnimatedSpritesheet) {
			auto ps = dynamic_cast<particles::TextureParticleSystem *>(particleSystem);
			ImGui::Checkbox("Additive blending", &ps->additiveBlendMode);
		}

		if (particleSystemMode == ParticleSystemMode::Texture) {
			auto ps = dynamic_cast<particles::TextureParticleSystem *>(particleSystem);

			const char *textureItems[] = { "Circle", "Blob", "Star" };
			static int textureItem = 0;
			if (ImGui::Combo("Texture", &textureItem, textureItems, 3)) {
				if (textureItem == 0) {
					ps->setTexture(circleTexture);
				}
				else if (textureItem == 1) {
					ps->setTexture(blobTexture);
				}
				if (textureItem == 2) {
					ps->setTexture(starTexture);
				}
			}
		}

		if (particleSystemMode == ParticleSystemMode::Metaball) {
			auto ps = dynamic_cast<particles::MetaballParticleSystem *>(particleSystem);
			ImGui::ColorEdit("Color", &ps->color);
			ImGui::SliderFloat("Threshold", &ps->threshold, 0.f, 0.999f);
		}
	}

	if (ImGui::CollapsingHeader("Particle Spawner")) {
		const char *spawnItems[] = { "Point", "Box", "Circle", "Disk" };
		static int spawnItem = 0;
		if (ImGui::Combo("Spawner Mode", &spawnItem, spawnItems, 4)) {
			spawnerMode = static_cast<SpawnerMode>(spawnItem);
			setSpawnMode();
		}

		if (spawnerMode == SpawnerMode::Point) {
			// nop
		}
		else if (spawnerMode == SpawnerMode::Box) {
			auto sp = dynamic_cast<particles::BoxSpawner *>(spawner);
			ImGui::SliderFloat2("size", &sp->size, 0.f, 500.f);
		}
		else if (spawnerMode == SpawnerMode::Circle) {
			auto sp = dynamic_cast<particles::CircleSpawner *>(spawner);
			ImGui::SliderFloat2("size", &sp->radius, 0.f, 500.f);
		}
		else if (spawnerMode == SpawnerMode::Disk) {
			auto sp = dynamic_cast<particles::DiskSpawner *>(spawner);
			ImGui::SliderFloat2("size", &sp->radius, 0.f, 500.f);
		}
	}

	if (ImGui::CollapsingHeader("Velocity Generator")) {
		const char *velItems[] = { "Angled", "Vector", "Aimed" };
		static int velItem = 0;
		if (ImGui::Combo("Velocity Mode", &velItem, velItems, 3)) {
			velocityGeneratorMode = static_cast<VelocityGeneratorMode>(velItem);
			setVelocityGeneratorMode();
		}

		if (velocityGeneratorMode == VelocityGeneratorMode::Angled) {
			auto gen = dynamic_cast<particles::AngledVelocityGenerator *>(velocityGenerator);
			ImGui::SliderFloat("min angle", &gen->minAngle, -180.f, 180.f);
			ImGui::SliderFloat("max angle", &gen->maxAngle, -180.f, 180.f);
			ImGui::SliderFloat("min speed", &gen->minStartSpeed, 0.f, 500.f);
			ImGui::SliderFloat("max speed", &gen->maxStartSpeed, 0.f, 500.f);
		}
		else if (velocityGeneratorMode == VelocityGeneratorMode::Vector) {
			auto gen = dynamic_cast<particles::VectorVelocityGenerator *>(velocityGenerator);
			ImGui::SliderFloat2("min", &gen->minStartVel, 0.f, 500.f);
			ImGui::SliderFloat2("max", &gen->maxStartVel, 0.f, 500.f);
		}
		else if (velocityGeneratorMode == VelocityGeneratorMode::Aimed) {
			auto gen = dynamic_cast<particles::AimedVelocityGenerator *>(velocityGenerator);
			ImGui::SliderFloat2("min", &gen->goal, 0.f, 500.f);
			ImGui::SliderFloat("min speed", &gen->minStartSpeed, 0.f, 500.f);
			ImGui::SliderFloat("max speed", &gen->maxStartSpeed, 0.f, 500.f);
		}
	}

	if (ImGui::CollapsingHeader("Time Generator")) {
		ImGui::SliderFloat("min", &timeGenerator->minTime, 0.f, 60.f);
		ImGui::SliderFloat("max", &timeGenerator->maxTime, 0.f, 60.f);
	}

	if (particleSystemMode != ParticleSystemMode::Points && ImGui::CollapsingHeader("Size Generator")) {
		ImGui::SliderFloat("min start size", &sizeGenerator->minStartSize, 0.f, 100.f);
		ImGui::SliderFloat("max start size", &sizeGenerator->maxStartSize, 0.f, 100.f);
		ImGui::SliderFloat("min end size", &sizeGenerator->minEndSize, 0.f, 100.f);
		ImGui::SliderFloat("max end size", &sizeGenerator->maxEndSize, 0.f, 100.f);
	}

	if (particleSystemMode != ParticleSystemMode::Points && ImGui::CollapsingHeader("Rotation Generator")) {
		ImGui::SliderFloat("min start angle", &rotationGenerator->minStartAngle, -180.f, 180.f);
		ImGui::SliderFloat("max start angle", &rotationGenerator->maxStartAngle, -180.f, 180.f);
		ImGui::SliderFloat("min end angle", &rotationGenerator->minEndAngle, -180.f, 180.f);
		ImGui::SliderFloat("max end angle", &rotationGenerator->maxEndAngle, -180.f, 180.f);
	}

	if (particleSystemMode != ParticleSystemMode::Metaball && ImGui::CollapsingHeader("Color Generator")) {
		ImGui::ColorEdit("min start", &colorGenerator->minStartCol);
		ImGui::ColorEdit("max start", &colorGenerator->maxStartCol);
		ImGui::ColorEdit("min end", &colorGenerator->minEndCol);
		ImGui::ColorEdit("max end", &colorGenerator->maxEndCol);
	}

	if (ImGui::CollapsingHeader("Euler Updater")) {
		ImGui::SliderFloat2("gravity", &eulerUpdater->globalAcceleration, 0.f, 200.f);
	}

	ImGui::End();
}

#include <filesystem>

#include <iostream>
#include <filesystem>

// Utility function to check file permissions
bool checkFilePermissions(const std::filesystem::path &filePath) {
	if (!std::filesystem::exists(filePath)) {
		std::cerr << "File does not exist: " << filePath << std::endl;
		return false;
	}

	auto perms = std::filesystem::status(filePath).permissions();

	std::cout << "Permissions for " << filePath << ":" << std::endl;

	if ((perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none) {
		std::cout << "  Owner: Read" << std::endl;
	}
	else {
		std::cout << "  Owner: No Read" << std::endl;
	}

	if ((perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none) {
		std::cout << "  Owner: Write" << std::endl;
	}
	else {
		std::cout << "  Owner: No Write" << std::endl;
	}

	if ((perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none) {
		std::cout << "  Owner: Execute" << std::endl;
	}
	else {
		std::cout << "  Owner: No Execute" << std::endl;
	}

	if ((perms & std::filesystem::perms::group_read) != std::filesystem::perms::none) {
		std::cout << "  Group: Read" << std::endl;
	}
	else {
		std::cout << "  Group: No Read" << std::endl;
	}

	if ((perms & std::filesystem::perms::group_write) != std::filesystem::perms::none) {
		std::cout << "  Group: Write" << std::endl;
	}
	else {
		std::cout << "  Group: No Write" << std::endl;
	}

	if ((perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none) {
		std::cout << "  Group: Execute" << std::endl;
	}
	else {
		std::cout << "  Group: No Execute" << std::endl;
	}

	if ((perms & std::filesystem::perms::others_read) != std::filesystem::perms::none) {
		std::cout << "  Others: Read" << std::endl;
	}
	else {
		std::cout << "  Others: No Read" << std::endl;
	}

	if ((perms & std::filesystem::perms::others_write) != std::filesystem::perms::none) {
		std::cout << "  Others: Write" << std::endl;
	}
	else {
		std::cout << "  Others: No Write" << std::endl;
	}

	if ((perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none) {
		std::cout << "  Others: Execute" << std::endl;
	}
	else {
		std::cout << "  Others: No Execute" << std::endl;
	}

	return true;
}

// NOTE(MO): I changed the sfml-...lib to sml-..-d.lib for debug
// Others have mentioned project -> properties -> configuration properties -> advanced -> use debugging libraries, 
// by default it is yes, you need to set it to default or "no". Translated through a translator, the translation may not be accurate

int main() {

	try {
		// Get the current working directory
		std::filesystem::path cwd = std::filesystem::current_path();

		// Print the current working directory
		std::cout << "Current working directory: " << cwd << std::endl;
	}
	catch (const std::filesystem::filesystem_error &e) {
		// Handle errors
		std::cerr << "Filesystem error: " << e.what() << std::endl;
	}

	auto s = "D:/DESKTOP/GLToys/LuaScripting2D/LuaScriptingEngine/ParticlesEditor/";

	circleTexture = new sf::Texture();
	blobTexture = new sf::Texture();
	starTexture = new sf::Texture();
	spritesheetTexture = new sf::Texture();

	std::filesystem::path filePath = s + std::string("circleTexture.png");
	std::filesystem::path PATH = s + std::string("circleTexture.png");
	if (!checkFilePermissions(PATH))
	{
		std::cerr << "File permission check failed." << std::endl;
		return -1;
	}

	if (std::filesystem::exists(filePath)) {
		std::cout << "File exists: " << filePath << std::endl;
	}
	else {
		std::cerr << "File does not exist: " << filePath << std::endl;
	}

	if (!circleTexture->loadFromFile(s + std::string("circleTexture.png"))) {
		std::cerr << "Failed to load circleTexture.png" << std::endl;
		return -1;
	}
	if (!blobTexture->loadFromFile(s + std::string("blobTexture.png"))) {
		std::cerr << "Failed to load blobTexture.png" << std::endl;
		return -1;
	}
	if (!starTexture->loadFromFile(s + std::string("starTexture.png"))) {
		std::cerr << "Failed to load starTexture.png" << std::endl;
		return -1;
	}
	if (!spritesheetTexture->loadFromFile(s + std::string("spritesheetTexture.png"))) {
		std::cerr << "Failed to load spritesheetTexture.png" << std::endl;
		return -1;
	}

	/*circleTexture->loadFromFile(s + std::string("circleTexture.png"));
	blobTexture->loadFromFile(s + std::string("blobTexture.png"));
	starTexture->loadFromFile(s + std::string("starTexture.png"));
	spritesheetTexture->loadFromFile(s + std::string("spritesheetTexture.png"));*/
	circleTexture->setSmooth(true);
	blobTexture->setSmooth(true);
	starTexture->setSmooth(true);
	spritesheetTexture->setSmooth(false);

	initParticleSystem();

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particles Demo");
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
				window.close();
			}
		}

		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(mouse);

		spawner->center = pos;

		sf::Time dt = clock.restart();
		ImGui::SFML::Update(window, dt);
		particleSystem->update(dt);

		gui();

		window.clear();

		particleSystem->render(window);
		//ImGui::Render();
		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
	delete particleSystem;
}


/*



int main()
{
	std::cout << "Particles Editor \n";

	sf::RenderWindow window(sf::VideoMode(800, 800), "Hello World");
	ImGui::SFML::Init(window);

	float circleRadius = 200;
	int circleSegments = 100;
	sf::CircleShape shape(circleRadius, circleSegments);


	float circleColor[3] = { 204.0f / 255, 77.0f / 255, 5.0f / 255 };

	shape.setFillColor(sf::Color(
		(int)(circleColor[0] * 255),
		(int)(circleColor[1] * 255),
		(int)(circleColor[2] * 255)
	));
	shape.setOrigin(circleRadius, circleRadius);
	shape.setPosition(400, 400);

	

	sf::Clock deltaClock;
	bool drawCircle = true;



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		ImGui::SFML::Update(window, deltaClock.restart());


		ImGui::Begin("Window Title");
		ImGui::Text("Window text!");
		ImGui::Checkbox("Circle", &drawCircle);
		ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
		ImGui::SliderInt("Segments", &circleSegments, 3, 150);
		ImGui::ColorEdit3("Color Circle", circleColor);
		ImGui::End();


		window.clear(sf::Color(18, 33, 43));

		
		
		shape.setRadius(circleRadius);
		shape.setOrigin(circleRadius, circleRadius);
		shape.setPointCount(circleSegments);
		shape.setFillColor(sf::Color(
			(int)(circleColor[0] * 255),
			(int)(circleColor[1] * 255),
			(int)(circleColor[2] * 255)
		));

		if(drawCircle)
			window.draw(shape);

		ImGui::SFML::Render(window);
		window.display();
	}


	ImGui::SFML::Shutdown();

	return 0;
}
*/