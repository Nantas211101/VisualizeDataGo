#pragma once
#include "ArrowData.hpp"
#include "Entity.hpp"
#include "NodeData.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "TextureManipulate.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

class StackNode : public SceneNode {
  public:
    explicit StackNode(const FontHolder& fonts, const TextureHolder& texture);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;
    // NodeData
    void setPosNode(sf::Vector2f pos);
    void setRadiusNode(float radius);
    void setNextNode(NodeData* mNext);
    void setValue(std::string value);
    auto getValue() -> std::string;

    // Arrow
    void setEnd(sf::Vector2f pos);
    void setIsDrawArrow(bool flag, int typee = 0);

    void setSelected();
    void setDeselected();
    void setHeadNode();
    void setLastNode(sf::Vector2f endpos = {0, 0});
    void desetLastNode();

  private:
    NodeData* mDataNode;
    ArrowData* mArrow;
};