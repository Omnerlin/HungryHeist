#include "HandSpawner.h"
#include "Physics.h"
#include "Assets.h"
#include "Game.h"

void HandSpawner::Initialize()
{
	handPositions = {
			// Bottom row
			HandSpawnPosition{ HandSpawnDirection::FromBottom, 32 - 8 },
			HandSpawnPosition{ HandSpawnDirection::FromBottom, 96 - 20 },
			HandSpawnPosition{ HandSpawnDirection::FromBottom, 160 - 32 },
			HandSpawnPosition{ HandSpawnDirection::FromBottom, 224 - 44 },
			HandSpawnPosition{ HandSpawnDirection::FromBottom, 288 - 56 },
			// Right Row
			HandSpawnPosition{ HandSpawnDirection::FromRight, 32 - 16 },
			HandSpawnPosition{ HandSpawnDirection::FromRight, 96 - 24 },
			HandSpawnPosition{ HandSpawnDirection::FromRight, 160 - 32 },
	};

	hands.reserve(handPositions.size());
	inactiveHandPositions.reserve(handPositions.size());
	for (size_t i = 0; i < handPositions.size(); i++) {
		Hand hand;
		hands.push_back(hand);
		Hand& newHand = hands[hands.size() - 1];
		newHand.grabTrigger.colliderType = ColliderType::Trigger;
		newHand.transform.SetWorldScale(0.5f, 0.5f);
		newHand.grabTrigger.drawable.setSize(sf::Vector2f(50, 75) * newHand.transform.GetWorldScale().x);
		newHand.handSprite.setTexture(Assets::LoadTexture("assets/textures/Hand.png"));
		newHand.exclamationSprite.setTexture(Assets::LoadTexture("assets/textures/Exclamation.png"));
		Physics::RegisterCollider(&newHand.grabTrigger);
		newHand.transform.SetAttachedTransformable(&newHand.handSprite);
		newHand.transform.SetOrigin(32, 0);
		newHand.grabTrigger.transform.SetParent(&newHand.transform);
		newHand.grabTrigger.transform.SetLocalPosition(0, 0);
	}
}

void HandSpawner::ResetHands()
{
	for (auto& hand : hands)
	{
		hand.capturedPlayer = false;
		hand.SetHandState(Hand::Retreating);
	}
}

void HandSpawner::Update(float deltaTime)
{

	auto generator = Game::Instance->randGenerator;
	auto mainCamera = Game::Instance->mainCamera;

	if(handSpawningEnabled)
	{
		timeSinceLastHand += deltaTime;
	}
	if (timeSinceLastHand >= timeBetweenHands) {
		timeSinceLastHand -= timeBetweenHands;
		if (!handPositions.empty()) {
			auto indexDistribution = std::uniform_int_distribution<int>(0, handPositions.size() - 1);
			int positionIndex = indexDistribution(generator);
			int handIndex = 0;
			for (size_t i = 0; i < hands.size(); i++) {
				if (hands[i].done) {
					handIndex = i;
					break;
				}
			}

			HandSpawnPosition spawnPosition = handPositions[positionIndex];
			handPositions.erase(handPositions.begin() + positionIndex);
			Hand* handToAttackWith = &hands[handIndex];

			handToAttackWith->HandFinishCallback = [this, spawnPosition]() {
				handPositions.push_back(spawnPosition);
			};
			handToAttackWith->handSprite.setColor(Hand::SkinColors[colorDistribution(generator)]);
			handToAttackWith->Attack(static_cast<HandSpawnDirection>(static_cast<int>(spawnPosition.direction) < 2
				? sideDistribution(generator)
				: topDistribution(generator)),
				mainCamera, 0.75f, spawnPosition.offset);
		}
	}
	for (auto& hand : hands)
	{
		hand.Update(deltaTime);
	}
}
