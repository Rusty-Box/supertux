#include "granito.hpp"

#include "math/random.hpp"

static const float WALK_SPEED = 80.f;

Granito::Granito(const ReaderMapping& reader):
  WalkingBadguy(reader, "images/creatures/granito/granito.sprite", "left", "right")
{
  parse_type(reader);

  walk_speed = WALK_SPEED;
  max_drop_height = 600;

  m_countMe = false;

}

void Granito::active_update(float dt_sec)
{
  switch (m_type) {
    case WALK: {
      if (!m_walk_interval.started() && !m_walk_interval.check())
      {
        m_walk_interval.start(gameRandom.rand(1000, 4000) / 1000);


        if (walk_speed > 0)
        {
          walk_speed = 0;
          m_physic.set_velocity_x(0);
          set_action("stand", m_dir);
        }
        else
        {
          m_dir = m_dir == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
          walk_speed = WALK_SPEED;
          m_physic.set_velocity_x(WALK_SPEED * (m_dir == Direction::LEFT ? -1 : 1));
          set_action(m_dir);
        }
      }

      break;
    }
  }

  WalkingBadguy::active_update(dt_sec);
}

void Granito::kill_fall()
{
  return;
}

GameObjectTypes Granito::get_types() const
{
  return {
    {"walking", _("Walking")},
    {"sitting", _("Sitting")},
    {"standing", _("Standing")}
  };
}

void Granito::initialize()
{
  WalkingBadguy::initialize();
  set_colgroup_active(COLGROUP_MOVING_ONLY_STATIC);
}
