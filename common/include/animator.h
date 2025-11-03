#pragma once

#include "animation.h"
#include <glm/glm.hpp>
#include <vector>

class Animator
{
public:
    explicit Animator(Animation* animation);

    void Update(float dt);
    void Play(Animation* pAnimation);

    void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform);

    [[nodiscard]] std::vector<glm::mat4> GetFinalBoneMatrices() const { return m_FinalBoneMatrices; }

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    double m_CurrentTime{};
    double m_DeltaTime{};
};