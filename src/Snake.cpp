#include "Snake.hpp"
#include <SDL2/SDL.h>
#include <iostream>

const int CELL_SIZE = 20;

Snake::Snake() {
    body.push_front({25, 25});
    body.push_back({24, 25});
    body.push_back({23, 25});
    
    direction = RIGHT;
}

void Snake::update() {
    Point currentHead = body.front();
    Point newHead = currentHead;

    switch (direction) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }

    body.push_front(newHead);
    body.pop_back();
}

void Snake::draw(SDL_Renderer *renderer) {
    if (body.empty()) {
        return;
    }

    Point head = body.front();
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    draw_body_part(renderer, head);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (auto it = body.begin() + 1; it != body.end(); ++it) {
        draw_body_part(renderer, *it);
    }
}

void Snake::draw_body_part(SDL_Renderer *renderer, Point part) {
    SDL_Rect rect = {
        part.x * CELL_SIZE, 
        part.y * CELL_SIZE, 
        CELL_SIZE, 
        CELL_SIZE
    };
    SDL_RenderFillRect(renderer, &rect);
}

bool Snake::hit_itself() {
    if(this->body.size() < 2) {
        return false;
    }

    Point head = this->body.front();
    for(size_t i = 1; i < this->body.size(); ++i) {
        if(head == this->body[i]) {
            return true;
        }
    }
    return false;
}

void Snake::grow() {
    body.push_back(body.back());
}

bool Snake::is_point_on_body(Point p, bool skip_tail = false) {
    auto start = body.begin() + 1;
    auto end = body.end();

    if (skip_tail && body.size() > 1) {
        --end;
    }

    for (auto it = start; it != end; ++it) {
        if (*it == p) {
            return true;
        }
    }
    return false;
}