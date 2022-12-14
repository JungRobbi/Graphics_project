#include "DestroyEffect.h"
#include "ItemRotate.h"

void DestroyEffect::update()
{
    if (!destroy)
        return;

    if (subObject) {
        --destroy_frame_time;
        if (destroy_frame_time <= 0)
            Scene::scene->PushDelete(gameObject);
        return;
    }

    for (int i{}; i < EFFECT_OBJECT_NUM; ++i) {
        auto box = Scene::scene->CreateEmpty();

        box->AddComponent<Transform3D>();
        box->AddComponent<Gravity>();
        box->AddComponent<DestroyEffect>();
        box->GetComponent<DestroyEffect>()->destroy = true;
        box->GetComponent<DestroyEffect>()->subObject = true;

        box->AddComponent<ItemRotate>();
        box->GetComponent<ItemRotate>()->rotating_axis[0] = true;
        box->GetComponent<ItemRotate>()->rotating_axis[2] = true;

        box->GetComponent<Transform3D>()->position = gameObject->GetComponent<Transform3D>()->position;
        box->GetComponent<Transform3D>()->scale.x = gameObject->GetComponent<Transform3D>()->scale.x * 0.3f;
        box->GetComponent<Transform3D>()->scale.y = gameObject->GetComponent<Transform3D>()->scale.y * 0.3f;
        box->GetComponent<Transform3D>()->scale.z = gameObject->GetComponent<Transform3D>()->scale.z * 0.3f;

        std::random_device rad;
        std::default_random_engine dre(rad());
        std::uniform_real_distribution<double> uid1{ -0.05, 0.05 };
        std::uniform_real_distribution<double> uid1abs{ 0.0, 0.05 };
        auto b = gameObject->GetComponent<Collide>()->BoundBox;
        std::uniform_real_distribution<double> uidx{ b.minX, b.maxX };
        std::uniform_real_distribution<double> uidy{ b.minY, b.maxY };
        std::uniform_real_distribution<double> uidz{ b.minZ, b.maxZ };
        float r_x = uidx(dre);
        float r_y = uidy(dre);
        float r_z = uidz(dre);
        
        box->GetComponent<Transform3D>()->position += glm::vec3(r_x, r_y, r_z);
        box->GetComponent<Transform3D>()->velocity = glm::vec3(uid1(dre), uid1abs(dre), uid1(dre));
        
        box->GetComponent<ItemRotate>()->rotating_speedx = uid1abs(dre) * 150;
        box->GetComponent<ItemRotate>()->rotating_speedy = uid1abs(dre) * 150;
        box->GetComponent<ItemRotate>()->rotating_speedz = uid1abs(dre) * 150;

        // render ????
        box->modelLocation = gameObject->modelLocation;
        box->num_index = gameObject->num_index; // load() ?? ????
        box->VAO = gameObject->VAO; // ?????? ????
        box->texture = gameObject->texture; // 1?? ??????
    }

    Scene::scene->PushDelete(gameObject);
}