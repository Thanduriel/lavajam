#include "controllercomponent.hpp"
#include "input/keymanager.hpp"
#include "glm.hpp"
#include "actors/particleactor.hpp"
#include "actors/characteractor.hpp"

ControllerComponent::ControllerComponent(Actor* actor, std::vector <int> keyMapping, bool isActive)
: CooldownComponent(actor, isActive), m_keyMapping(keyMapping),
m_fireMode(FireMode::Simple)
{};

void ControllerComponent::Process(float deltaTime)
{
    if (!this->GetActor()->GetWin())
    {
        CooldownComponent::Process(deltaTime);
        float translate = 0;
        float rotate = 0;
        if(Input::KeyManager::getKeyStates(m_keyMapping[0])!=Input::EKeyState::RELEASE) translate-=deltaTime;
        if(Input::KeyManager::getKeyStates(m_keyMapping[2])!=Input::EKeyState::RELEASE) translate+=deltaTime;
        if(Input::KeyManager::getKeyStates(m_keyMapping[1])!=Input::EKeyState::RELEASE) rotate-=deltaTime;
        if(Input::KeyManager::getKeyStates(m_keyMapping[3])!=Input::EKeyState::RELEASE) rotate+=deltaTime;
        m_actor->AddRotation(rotate * 5);
        m_actor->AddVelocity(glm::rotate(glm::vec2(0, translate / 1.f), m_actor->GetRotation()));   
        if(Input::KeyManager::getKeyStates(m_keyMapping[4])!=Input::EKeyState::RELEASE) {
			if (this->m_spawnCallback != nullptr && this->GetCooldown())
			{

				SetCooldown(0.25f);
				switch (m_fireMode)
				{
				case FireMode::Simple:
					FireBullet(1.f, glm::vec2{}, 0.f);
					break;
				case FireMode::Double:
					FireBullet(1.f, glm::vec2(0.02f, 0.f), 0.f);
					FireBullet(1.f, glm::vec2(-0.02f, 0.f), 0.f);
					break;
				case FireMode::Spread:
					FireBullet(1.f, glm::vec2{}, 0.f);
					FireBullet(1.f, glm::vec2{}, 0.2f);
					FireBullet(1.f, glm::vec2{}, -0.2f);
				}
			}
		}
    }

	if (this->m_spawnCallback != nullptr && this->m_particleCooldown <= 0)
	{
		this->m_particleCooldown = 0.01f;
		for (auto i = 0; i < std::round(std::rand() / static_cast<float>(RAND_MAX) * 20.0f); i++) {
			auto actor = static_cast<CharacterActor*>(this->GetActor());
			auto x = (std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * actor->GetDrawComponent().GetSize();
			auto y = (std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) * actor->GetDrawComponent().GetSize();
			auto r = std::rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();
			auto particle = new ParticleActor(0.001f, actor->GetDrawComponent().GetColor(), 0, actor->GetPosition() + glm::vec2(x, y), r, glm::vec2(y, x), 1-length(actor->GetVelocity()/2.f)+std::rand() / static_cast<float>(RAND_MAX));
			this->m_spawnCallback(particle);
		}
	}

    this->m_particleCooldown -= deltaTime * length(this->GetActor()->GetVelocity());
}


void ControllerComponent::FireBullet(float _speed, glm::vec2 _offset, float _rot)
{
	static const glm::vec4 white(1, 1, 1, 1);
	static const glm::vec4 brown(141 / 255.f, 110 / 255.f, 99 / 255.f, 1);
	const float rot = m_actor->GetRotation() + _rot;
	const glm::vec2 velocity = glm::rotate(glm::vec2(0.f, _speed + glm::length(m_actor->GetVelocity())), m_actor->GetRotation() + _rot);
	const auto bullet = new BulletActor(0.006f, white, 0, m_actor->GetPosition() - normalize(velocity) / 15 + glm::rotate(_offset, rot), rot, -velocity);
	m_spawnCallback(bullet);
}
