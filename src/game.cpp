#include "game.hpp"

void Game::Update(const glm::vec2& position) {
    Render_(position);
}

void Game::StartGame() {

}

void Game::InitGame() {
    render_->InitWindow();
    render_->InitRender();
}

void Game::Render_(const glm::vec2& position) {
    render_->Draw(position);
}