#include "game/objects/common.h"

#include "game/collide.h"
#include "game/inventory.h"
#include "game/items.h"
#include "game/lara/common.h"
#include "game/objects/vars.h"
#include "game/output.h"
#include "game/room.h"
#include "game/viewport.h"
#include "global/const.h"
#include "global/vars.h"
#include "math/matrix.h"

#include <libtrx/config.h>
#include <libtrx/debug.h>
#include <libtrx/utils.h>

#include <stddef.h>

OBJECT *Object_GetObject(GAME_OBJECT_ID object_id)
{
    return &g_Objects[object_id];
}

int16_t Object_FindReceptacle(GAME_OBJECT_ID object_id)
{
    GAME_OBJECT_ID receptacle_to_check =
        Object_GetCognate(object_id, g_KeyItemToReceptacleMap);
    for (int item_num = 0; item_num < g_LevelItemCount; item_num++) {
        ITEM *item = &g_Items[item_num];
        if (item->object_id == receptacle_to_check) {
            const OBJECT *const obj = &g_Objects[item->object_id];
            if (obj->is_usable != NULL && !obj->is_usable(item_num)) {
                continue;
            }
            if (Lara_TestPosition(item, obj->bounds())) {
                return item_num;
            }
        }
    }

    return NO_OBJECT;
}

void Object_Collision(int16_t item_num, ITEM *lara_item, COLL_INFO *coll)
{
    ITEM *item = &g_Items[item_num];

    if (!Lara_TestBoundsCollide(item, coll->radius)) {
        return;
    }
    if (!Collide_TestCollision(item, lara_item)) {
        return;
    }

    if (coll->enable_baddie_push) {
        Lara_Push(item, coll, 0, 1);
    }
}

void Object_CollisionTrap(int16_t item_num, ITEM *lara_item, COLL_INFO *coll)
{
    ITEM *item = &g_Items[item_num];

    if (item->status == IS_ACTIVE) {
        if (Lara_TestBoundsCollide(item, coll->radius)) {
            Collide_TestCollision(item, lara_item);
        }
    } else if (item->status != IS_INVISIBLE) {
        Object_Collision(item_num, lara_item, coll);
    }
}

void Object_DrawDummyItem(ITEM *item)
{
}

void Object_DrawSpriteItem(ITEM *item)
{
    Output_DrawSprite(
        item->interp.result.pos.x, item->interp.result.pos.y,
        item->interp.result.pos.z,
        g_Objects[item->object_id].mesh_idx - item->frame_num, item->shade);
}

void Object_DrawPickupItem(ITEM *item)
{
    if (!g_Config.visuals.enable_3d_pickups) {
        Object_DrawSpriteItem(item);
        return;
    }

    // Convert item to menu display item.
    int16_t item_num_option = Inv_GetItemOption(item->object_id);
    // Save the frame number.
    int16_t old_frame_num = item->frame_num;
    // Modify item to be the anim for inv item and animation 0.
    Item_SwitchToObjAnim(item, 0, 0, item_num_option);

    const OBJECT *const object = Object_GetObject(item_num_option);
    const ANIM_FRAME *frame = Item_GetAnim(item)->frame_ptr;

    // Restore the old frame number in case we need to get the sprite again.
    item->frame_num = old_frame_num;

    // Fall back to normal sprite rendering if not found.
    if (object->mesh_count < 0) {
        Object_DrawSpriteItem(item);
        return;
    }

    // Good news is there is a mesh, we just need to work out where to put it

    // First - Is there floor under the item?
    // This is mostly true, but for example the 4 items in the Obelisk of
    // Khamoon the 4 items are sitting on top of a static mesh which is not
    // floor.
    const SECTOR *const sector =
        Room_GetSector(item->pos.x, item->pos.y, item->pos.z, &item->room_num);
    const int16_t floor_height =
        Room_GetHeight(sector, item->pos.x, item->pos.y, item->pos.z);

    // Assume this is our offset.
    int16_t offset = floor_height;
    // Is the floor "just below" the item?
    int16_t floor_mapped_delta = ABS(floor_height - item->pos.y);
    if (floor_mapped_delta > WALL_L / 4 || floor_mapped_delta == 0) {
        // No, now we need to move it a bit.
        // First get the sprite that was to be used,

        int16_t spr_num = g_Objects[item->object_id].mesh_idx - item->frame_num;
        PHD_SPRITE *sprite = &g_PhdSpriteInfo[spr_num];

        // and get the animation bounding box, which is not the mesh one.
        int16_t min_y = frame->bounds.min.y;
        int16_t max_y = frame->bounds.max.y;
        int16_t anim_y = frame->offset.y;

        // Different objects need different heuristics.
        switch (item_num_option) {
        case O_PISTOL_OPTION:
        case O_SHOTGUN_OPTION:
        case O_MAGNUM_OPTION:
        case O_UZI_OPTION:
        case O_MAG_AMMO_OPTION:
        case O_UZI_AMMO_OPTION:
        case O_EXPLOSIVE_OPTION:
        case O_LEADBAR_OPTION:
        case O_PICKUP_OPTION_1:
        case O_PICKUP_OPTION_2:
        case O_SCION_OPTION:
            // Ignore the sprite and just position based upon the anim.
            offset = item->pos.y + (min_y - anim_y) / 2;
            break;
        case O_MEDI_OPTION:
        case O_BIGMEDI_OPTION:
        case O_SG_AMMO_OPTION:
        case O_PUZZLE_OPTION_1:
        case O_PUZZLE_OPTION_2:
        case O_PUZZLE_OPTION_3:
        case O_PUZZLE_OPTION_4:
        case O_KEY_OPTION_1:
        case O_KEY_OPTION_2:
        case O_KEY_OPTION_3:
        case O_KEY_OPTION_4: {
            // Take the difference from the bottom of the sprite and the bottom
            // of the animation and divide it by 8.
            // 8 was chosen because in testing it positioned objects correctly.
            // Specifically the 4 items in the Obelisk of Khamoon and keys.
            // Some objects have a centred mesh and some have one that is from
            // the bottom, for the centred ones; move up from the
            // bottom is necessary.
            int centred = ABS(min_y + max_y) < 8;
            if (floor_mapped_delta) {
                offset = item->pos.y - ABS(min_y - sprite->y1) / 8;
            } else if (centred) {
                offset = item->pos.y + min_y;
            }
            break;
        }
        }
    }

    Matrix_Push();
    Matrix_TranslateAbs(
        item->interp.result.pos.x, offset, item->interp.result.pos.z);
    Matrix_RotYXZ(
        item->interp.result.rot.y, item->interp.result.rot.x,
        item->interp.result.rot.z);

    Output_CalculateLight(
        item->pos.x, item->pos.y, item->pos.z, item->room_num);

    frame = object->frame_base;
    int32_t clip = Output_GetObjectBounds(&frame->bounds);
    if (clip) {
        // From this point on the function is a slightly customised version
        // of the code in DrawAnimatingItem starting with the line that
        // matches the following line.
        int32_t bit = 1;

        Matrix_TranslateRel(frame->offset.x, frame->offset.y, frame->offset.z);

        Matrix_RotXYZ16(&frame->mesh_rots[0]);

        if (item->mesh_bits & bit) {
            Object_DrawMesh(object->mesh_idx, clip, false);
        }

        for (int i = 1; i < object->mesh_count; i++) {
            const ANIM_BONE *const bone = Object_GetBone(object, i - 1);
            if (bone->matrix_pop) {
                Matrix_Pop();
            }

            if (bone->matrix_push) {
                Matrix_Push();
            }

            Matrix_TranslateRel(bone->pos.x, bone->pos.y, bone->pos.z);
            Matrix_RotXYZ16(&frame->mesh_rots[i]);

            // Extra rotation is ignored in this case as it's not needed.

            bit <<= 1;
            if (item->mesh_bits & bit) {
                Object_DrawMesh(object->mesh_idx + i, clip, false);
            }
        }
    }

    Matrix_Pop();
}

void Object_DrawInterpolatedObject(
    const OBJECT *const object, uint32_t meshes, const int16_t *extra_rotation,
    const ANIM_FRAME *const frame1, const ANIM_FRAME *const frame2,
    const int32_t frac, const int32_t rate)
{
    ASSERT(frame1 != NULL);
    int32_t clip = Output_GetObjectBounds(&frame1->bounds);
    if (!clip) {
        return;
    }

    Matrix_Push();
    int32_t mesh_num = 1;

    ASSERT(rate != 0);
    if (!frac) {
        Matrix_TranslateRel(
            frame1->offset.x, frame1->offset.y, frame1->offset.z);

        Matrix_RotXYZ16(&frame1->mesh_rots[0]);

        if (meshes & mesh_num) {
            Object_DrawMesh(object->mesh_idx, clip, false);
        }

        for (int i = 1; i < object->mesh_count; i++) {
            const ANIM_BONE *const bone = Object_GetBone(object, i - 1);
            if (bone->matrix_pop) {
                Matrix_Pop();
            }

            if (bone->matrix_push) {
                Matrix_Push();
            }

            Matrix_TranslateRel(bone->pos.x, bone->pos.y, bone->pos.z);
            Matrix_RotXYZ16(&frame1->mesh_rots[i]);

            if (extra_rotation != NULL) {
                if (bone->rot_y) {
                    Matrix_RotY(*extra_rotation++);
                }
                if (bone->rot_x) {
                    Matrix_RotX(*extra_rotation++);
                }
                if (bone->rot_z) {
                    Matrix_RotZ(*extra_rotation++);
                }
            }

            mesh_num <<= 1;
            if (meshes & mesh_num) {
                Object_DrawMesh(object->mesh_idx + i, clip, false);
            }
        }
    } else {
        ASSERT(frame2 != NULL);
        Matrix_InitInterpolate(frac, rate);
        Matrix_TranslateRel_ID(
            frame1->offset.x, frame1->offset.y, frame1->offset.z,
            frame2->offset.x, frame2->offset.y, frame2->offset.z);
        Matrix_RotXYZ16_I(&frame1->mesh_rots[0], &frame2->mesh_rots[0]);

        if (meshes & mesh_num) {
            Object_DrawMesh(object->mesh_idx, clip, true);
        }

        for (int i = 1; i < object->mesh_count; i++) {
            const ANIM_BONE *const bone = Object_GetBone(object, i - 1);
            if (bone->matrix_pop) {
                Matrix_Pop_I();
            }

            if (bone->matrix_push) {
                Matrix_Push_I();
            }

            Matrix_TranslateRel_I(bone->pos.x, bone->pos.y, bone->pos.z);
            Matrix_RotXYZ16_I(&frame1->mesh_rots[i], &frame2->mesh_rots[i]);

            if (extra_rotation != NULL) {
                if (bone->rot_y) {
                    Matrix_RotY_I(*extra_rotation++);
                }
                if (bone->rot_x) {
                    Matrix_RotX_I(*extra_rotation++);
                }
                if (bone->rot_z) {
                    Matrix_RotZ_I(*extra_rotation++);
                }
            }

            mesh_num <<= 1;
            if (meshes & mesh_num) {
                Object_DrawMesh(object->mesh_idx + i, clip, true);
            }
        }
    }

    Matrix_Pop();
}

void Object_DrawAnimatingItem(ITEM *item)
{
    ANIM_FRAME *frmptr[2];
    int32_t rate;
    int32_t frac = Item_GetFrames(item, frmptr, &rate);
    OBJECT *object = &g_Objects[item->object_id];

    if (object->shadow_size) {
        Output_DrawShadow(object->shadow_size, &frmptr[0]->bounds, item);
    }

    Matrix_Push();
    Matrix_TranslateAbs(
        item->interp.result.pos.x, item->interp.result.pos.y,
        item->interp.result.pos.z);
    Matrix_RotYXZ(
        item->interp.result.rot.y, item->interp.result.rot.x,
        item->interp.result.rot.z);

    Output_CalculateObjectLighting(item, &frmptr[0]->bounds);
    const int16_t *extra_rotation = item->data ? item->data : NULL;

    Object_DrawInterpolatedObject(
        &g_Objects[item->object_id], item->mesh_bits, extra_rotation, frmptr[0],
        frmptr[1], frac, rate);
    Matrix_Pop();
}

void Object_DrawUnclippedItem(ITEM *item)
{
    int32_t left = g_PhdLeft;
    int32_t top = g_PhdTop;
    int32_t right = g_PhdRight;
    int32_t bottom = g_PhdBottom;

    g_PhdLeft = Viewport_GetMinX();
    g_PhdTop = Viewport_GetMinY();
    g_PhdRight = Viewport_GetMaxX();
    g_PhdBottom = Viewport_GetMaxY();

    Object_DrawAnimatingItem(item);

    g_PhdLeft = left;
    g_PhdTop = top;
    g_PhdRight = right;
    g_PhdBottom = bottom;
}

void Object_SetMeshReflective(
    const GAME_OBJECT_ID object_id, const int32_t mesh_idx, const bool enabled)
{
    const OBJECT *const object = Object_GetObject(object_id);
    if (!object->loaded) {
        return;
    }

    OBJECT_MESH *const mesh = Object_GetMesh(object->mesh_idx + mesh_idx);
    mesh->enable_reflections = enabled;

    for (int32_t i = 0; i < mesh->num_tex_face4s; i++) {
        mesh->tex_face4s[i].enable_reflections = enabled;
    }

    for (int32_t i = 0; i < mesh->num_tex_face3s; i++) {
        mesh->tex_face3s[i].enable_reflections = enabled;
    }

    for (int32_t i = 0; i < mesh->num_flat_face4s; i++) {
        mesh->flat_face4s[i].enable_reflections = enabled;
    }

    for (int32_t i = 0; i < mesh->num_flat_face3s; i++) {
        mesh->flat_face3s[i].enable_reflections = enabled;
    }
}

void Object_SetReflective(const GAME_OBJECT_ID object_id, const bool enabled)
{
    const OBJECT *const object = Object_GetObject(object_id);
    for (int32_t i = 0; i < object->mesh_count; i++) {
        Object_SetMeshReflective(object_id, i, enabled);
    }
}

void Object_DrawMesh(
    const int32_t mesh_idx, const int32_t clip, const bool interpolated)
{
    const OBJECT_MESH *const mesh = Object_GetMesh(mesh_idx);
    if (interpolated) {
        Output_DrawObjectMesh_I(mesh, clip);
    } else {
        Output_DrawObjectMesh(mesh, clip);
    }
}
