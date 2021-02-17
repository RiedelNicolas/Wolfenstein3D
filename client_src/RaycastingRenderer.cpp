#include "RaycastingRenderer.h"
#include "Raycaster.h"
#include "textures/TextureID.h"
#include "../common_src/GameConstants.h"
#include "Exceptions/RaycastingException.h"

#define VISUAL_PROPORTION 1.5

RaycastingRenderer::RaycastingRenderer(TexturesContainer &textures, Map &map,
                                       std::unordered_map<int, SlidingSurface> *surfaces,
                                       int begin_x, int begin_y, int width, int height):
                                       textures(textures), map(map), surfaces(surfaces),
                                       width(width), height(height), begin_x(begin_x), begin_y(begin_y){
}

void RaycastingRenderer::updateSurfaces(
        std::unordered_map<int, SlidingSurface> *new_surfaces) {
    this->surfaces = new_surfaces;
}

void RaycastingRenderer::render(float wall_dist, char hit_axis, int ray_number,
                                const DirectedPositionable &player,
                                RayDirection ray_dir, int map_x, int map_y) {
    int line_height = (int)(height/(wall_dist * VISUAL_PROPORTION));

    int draw_start = (-line_height/2) + (height/2);
    int draw_end = (line_height/2) + (height/2);
    if (draw_start < 0) draw_start = 0;
    if (draw_end >= height) draw_end = height - 1;

    int tex_id = map.get(map_x, map_y);

    int tex_x = _calculateTextureXCoordinate(player, ray_dir, wall_dist,
                                             hit_axis, ray_number);
    if(tex_id == DOOR || tex_id == PASSAGE)
        _processSurfaceTexture(tex_x, tex_id, map_x, map_y, ray_dir, hit_axis);

    Texture* texture = textures.getStatic(TextureID(tex_id));
    SDL_Rect tex_portion;

    if (line_height <= height){
        tex_portion =  {tex_x, 0, 1, TEX_HEIGHT};
    } else {
        int portion = ((line_height-height)*TEX_HEIGHT)/(2*line_height);
        tex_portion =  {tex_x, portion, 1, TEX_HEIGHT-2*portion};
    }

    SDL_Rect stretched = {ray_number + begin_x, draw_start + begin_y,
                          1,draw_end-draw_start};
    if (hit_axis == 'y'){
        texture->changeColorModulation(SHADE_TEX);
    }
    texture->render(&tex_portion, &stretched);
    texture->changeColorModulation(ORIGINAL_TEX); // vuelvo al color original
}

// ------------------------- Metodos privados --------------------------------//

// Calcula la coordenada x de la textura que se debe graficar.
int RaycastingRenderer::_calculateTextureXCoordinate(const DirectedPositionable& player,
                                            RayDirection ray_dir, float wall_dist,
                                            char hit_axis, int ray_number) const {
    double wall_x;
    if (hit_axis == 'x')
        wall_x = player.getY() + wall_dist * ray_dir.y;
    else
        wall_x = player.getX() + wall_dist * ray_dir.x;
    wall_x -= int((wall_x));

    int tex_x = int((wall_x+ray_number/width) * TEX_WIDTH);
    if (hit_axis == 'x' && ray_dir.x > 0)
        tex_x = TEX_WIDTH - tex_x - 1;
    if (hit_axis == 'y' && ray_dir.y < 0)
        tex_x = TEX_WIDTH - tex_x - 1;

    return tex_x;
}

// Reposiciona el inicio de la textura de la puerta de acuerdo al estado de la misma.
void RaycastingRenderer::_processSurfaceTexture(int &tex_x, int &tex_id, int map_x,
                                                int map_y, RayDirection ray_dir, char hit_axis) {
    if (surfaces == nullptr){
        throw RaycastingException("Invalid sliding surfaces to render.");
    }
    int id;
    for(auto& surface: *surfaces){
        if(surface.second.getX() == map_x && surface.second.getY() == map_y)
            id = surface.first;
    }
    SlidingSurface surface = surfaces->at(id);

    if ((hit_axis == 'x' && ray_dir.x>0) || (hit_axis == 'y' && ray_dir.y<0)){
        if (surface.isOpening()){
            tex_x -= surface.getElapsedFraction()*TEX_WIDTH;
        } else if(surface.isClosing()){
            tex_x -= (1 - surface.getElapsedFraction())*TEX_WIDTH;
        }
    } else {
        if (surface.isOpening()){
            tex_x += surface.getElapsedFraction()*TEX_WIDTH;
        } else if(surface.isClosing()){
            tex_x += (1 - surface.getElapsedFraction())*TEX_WIDTH;
        }
        if(surface.getSurfaceType() == DOOR) tex_id = int(InvertedDoor);
    }
    if(surface.getSurfaceType() == PASSAGE) tex_id = int(Wall);
}
