#include "game.hpp"

void Game::Update(const glm::vec2& position) {

    GLfloat x = player_->GetPos().x + position.x;
    GLfloat y = player_->GetPos().y + position.y;
    player_->SetPosX(x);
    player_->SetPosY(y);

    glm::vec2 size(player_->GetSize().x, player_->GetSize().y);
    Render_(glm::vec2(player_->GetPos().x, player_->GetPos().y), size, 0.f);
}

void Game::StartGame() {
    render_->SetOrthoProjection(0.f, 0.f, 0.f, 0.f, -50.f, 50.f);

    glm::vec2 size(player_->GetSize().x, player_->GetSize().y);

    Render_(glm::vec2(player_->GetPos().x, player_->GetPos().y), size, 0.f);
}

void Game::InitGame() {
    render_->InitWindow();
    render_->InitRender();
}

void Game::Render_(const glm::vec2& position, const glm::vec2& size, float rotate) {
    render_->Draw(position, size, rotate);
}