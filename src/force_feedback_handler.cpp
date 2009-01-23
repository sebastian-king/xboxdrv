/* 
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <assert.h>
#include "force_feedback_handler.hpp"

std::ostream& operator<<(std::ostream& out, const struct ff_envelope& envelope)
{
  out << "Envelope(attack_length:" << envelope.attack_length
      << ", attack_level:" << envelope.attack_level
      << ", fade_length:" << envelope.fade_length
      << ", fade_level:" << envelope.fade_level << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const struct ff_replay& replay)
{
  out << "Replay(length:" << replay.length << ", delay:" << replay.delay << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const struct ff_trigger& trigger)
{
  out << "Trigger(button:" << trigger.button << ", interval:" << trigger.interval << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const struct ff_effect& effect)
{
  std::cout << "Effect(";
  switch (effect.type)
    {
      case FF_CONSTANT:
        out << "FF_CONSTANT("
            << "level:" << effect.u.constant.level
            << ", envelope:" << effect.u.constant.envelope << ")";
        break;

      case FF_PERIODIC:
        out << "FF_PERIODIC("
            << ", waveform:" << effect.u.periodic.waveform
            << ", period:" << effect.u.periodic.period
            << ", magnitude:" << effect.u.periodic.magnitude
            << ", offset:" << effect.u.periodic.offset
            << ", phase:" << effect.u.periodic.phase
            << ", envelope:" << effect.u.periodic.envelope << ")";
        break;

      case FF_RAMP:
        out << "FF_RAMP(" 
            << "start_level:" << effect.u.ramp.start_level
            << ", end_level:" << effect.u.ramp.end_level
            << ", envelope:" <<  effect.u.ramp.envelope << ")";
        break;

      case FF_SPRING:
        out << "FF_SPRING()";
        break;

      case FF_FRICTION:
        out << "FF_FRICTION()";
        break;

      case FF_DAMPER:
        out << "FF_DAMPER()";
        break;

      case FF_RUMBLE:
        out << "FF_RUMBLE("
            << "strong_magnitude:" << effect.u.rumble.strong_magnitude
            << ", weak_magnitude:" << effect.u.rumble.weak_magnitude << ")";
        break;

      case FF_INERTIA:
        out << "FF_INERTIA()";
        break;

      case FF_CUSTOM:
        out << "FF_CUSTOM()";
        break;

      default:
        out << "FF_<unknown>()";
        break;
    }

  out << ", direction:" << effect.direction
      << ", replay:" << effect.replay
      << ", trigger:" << effect.trigger << ")";

  return out;
}

ForceFeedbackEffect::ForceFeedbackEffect(const struct ff_effect& effect)
{
  delay  = effect.replay.delay;
  length = effect.replay.length;

  switch(effect.type)
    {
      case FF_CONSTANT:
        start_weak_magnitude   = effect.u.constant.level;
        start_strong_magnitude = effect.u.constant.level;
        end_weak_magnitude     = effect.u.constant.level;
        end_strong_magnitude   = effect.u.constant.level;

        envelope = effect.u.constant.envelope;
        break;

      case FF_PERIODIC:
        start_weak_magnitude   = effect.u.periodic.magnitude;
        start_strong_magnitude = effect.u.periodic.magnitude;
        end_weak_magnitude     = effect.u.periodic.magnitude;
        end_strong_magnitude   = effect.u.periodic.magnitude;

        envelope = effect.u.periodic.envelope;
        break;

      case FF_RAMP:
        start_weak_magnitude   = effect.u.ramp.start_level;
        start_strong_magnitude = effect.u.ramp.start_level;
        end_weak_magnitude     = effect.u.ramp.end_level;
        end_strong_magnitude   = effect.u.ramp.end_level;

        envelope = effect.u.ramp.envelope;
        break;

      case FF_RUMBLE:
        start_weak_magnitude   = effect.u.rumble.weak_magnitude;
        start_strong_magnitude = effect.u.rumble.strong_magnitude;
        end_weak_magnitude     = effect.u.rumble.weak_magnitude;
        end_strong_magnitude   = effect.u.rumble.strong_magnitude;
        break;
        
      default:
        // Unsupported effects
        // case FF_SPRING:
	// case FF_FRICTION:
        // case FF_DAMPER
        // case FF_INERTIA:
        // case FF_CUSTOM:
        assert(!"Unsupported effect");
        break;
    }
}

ForceFeedbackHandler::ForceFeedbackHandler()
  : max_effects(16),
    weak_magnitude(0),
    strong_magnitude(0)
{
  
}

ForceFeedbackHandler::~ForceFeedbackHandler()
{
  
}

int
ForceFeedbackHandler::get_max_effects()
{
  return max_effects;
}

void
ForceFeedbackHandler::upload(const struct ff_effect& effect)
{
  std::cout << "FF_UPLOAD("
            << "effect_id:" << effect.id
            << ", effect_type:" << effect.type
            << ",\n          "  << effect 
            << ")" << std::endl;
  effects[effect.id] = ForceFeedbackEffect(effect);
}

void
ForceFeedbackHandler::erase(int id)
{
  std::cout << "FF_ERASE(effect_id:" << id << ")" << std::endl;

  std::map<int, ForceFeedbackEffect>::iterator i = effects.find(id);
  if (i != effects.end())
    effects.erase(i);
  else
    std::cout << "ForceFeedbackHandler::erase: Unknown id " << id << std::endl;
}

void
ForceFeedbackHandler::play(int id)
{
  std::cout << "FFPlay(effect_id:" << id << ")" << std::endl;

  std::map<int, ForceFeedbackEffect>::iterator i = effects.find(id);
  if (i != effects.end())
    ; // play
  else
    std::cout << "ForceFeedbackHandler::play: Unknown id " << id << std::endl;
}

void
ForceFeedbackHandler::stop(int id)
{
  std::cout << "FFStop(effect_id:" << id << ")" << std::endl;

  std::map<int, ForceFeedbackEffect>::iterator i = effects.find(id);
  if (i != effects.end())
    ; // stop
  else
    std::cout << "ForceFeedbackHandler::play: Unknown id " << id << std::endl;
}

void
ForceFeedbackHandler::update(int msec_delta)
{
  for(Effects::iterator i = effects.begin(); i != effects.end(); ++i)
    {
      
    }
}

int
ForceFeedbackHandler::get_weak_magnitude() const
{
  return weak_magnitude;
}

int
ForceFeedbackHandler::get_strong_magnitude() const
{
  return strong_magnitude;
}

/* EOF */