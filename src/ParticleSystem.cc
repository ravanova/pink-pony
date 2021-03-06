#include "ParticleSystem.hh"
#include <ImathRandom.h>
#include "Camera.hh"
#include <ImathColorAlgo.h>

using std::set;


ParticleSystem::~ParticleSystem()
{
    for (set<ParticleSource*>::iterator i = sources.begin();
         i != sources.end(); ++i) {
        (*i)->remove_link();
    }
}

ParticleSystem* ParticleSystem::make_particle_system(GLuint max_particles,
                                                     Config* config)
{
    if (!config->use_particles) {
        return new InactiveParticleSystem(max_particles, config);
    } else if (!FLEXT_ARB_geometry_shader4 || !FLEXT_EXT_transform_feedback) {
        cout << "No hardware accelerated particles. :("
             << " Using CPU fallback." << endl;
        return new CPUParticleSystem(max_particles/10, config);
    } else {
        cout << "Hardware accelerated particles supported! :D" << endl;
        return new TransformFeedbackParticleSystem(max_particles, config);
    }
}


void InactiveParticleSystem::step_simulation(float time_diff)
{
    Particle p;

    for (set<ParticleSource*>::iterator i = sources.begin();
         i != sources.end(); ++i) {
        while ((*i)->has_particle()) {
            (*i)->get_particle(p); // Empty pending particles.
        }
    }
}

CPUParticleSystem::CPUParticleSystem
    (GLuint max_particles, Config* config)
        : last_stat_print(glfwGetTime()),
          draw_shader(config->resource_dir + "GLSL/draw_particles_cpu"),
          particle_tex((config->resource_dir + "textures/heart-particle.png").c_str()),
          heightmap((config->resource_dir + config->heightmap_file).c_str()),
          level_size(config->level_size),
          water_level(config->water_level),
          rel_water_level(0),
          particle_count(0)
{
    heightmap.normalize();

    rel_water_level = (water_level-level_size.min.y)/level_size.size().y;
    particles.resize(1000);
}


void CPUParticleSystem::step_simulation(float time_diff)
{
    for (set<ParticleSource*>::iterator i = sources.begin();
         i != sources.end(); ++i) {
        while ((*i)->has_particle()) {
            ++particle_count;
            if (particle_count > particles.size()) {
                particles.resize(particle_count*2);
            }
            (*i)->get_particle(particles[particle_count-1]);
        }
    }

    Particle* p = &(particles[0]);

    int write = 0;
    for (unsigned read = 0; read < particle_count; ++read) {
        if (step_particle(time_diff, p[read], p[write])) {
            ++write;
        }
    }

    particle_count = write;


    if (glfwGetTime() - last_stat_print > 10.0) {
        last_stat_print = glfwGetTime();

        cout << particle_count << " particles." << endl;
    }
}

bool CPUParticleSystem::step_particle(float time_diff,
                                      Particle& in, 
                                      Particle& out) 
{
    // Take care: in can reference the same position as out!
    
    if (in.life <= time_diff) return false;
    
    
    out.vel = in.vel + time_diff * V3f(0,-9.81 * 2, 0);
    out.pos = in.pos + out.vel * time_diff;
    out.color = in.color / in.life * (in.life - time_diff);
    out.life = in.life-time_diff;
    
    V3f rel_pos = box_unmap(level_size, out.pos);

    V2f uv(rel_pos.x, 1-rel_pos.z); 
    float rel_level = heightmap.get_value(uv);

    if (rel_water_level > rel_level && rel_pos.y < rel_water_level) {
        out.pos.y = water_level;
        out.vel = (reflect(out.vel.normalized(), V3f(0,1,0)) * 
                   out.vel.length() * 0.25);
    } else if (rel_pos.y < rel_level) {
        out.pos.y = (rel_level * level_size.size().y) + level_size.min.y;
        
        V3f dx = rel_pos + V3f(0.01,0,0);
        V3f dy = rel_pos + V3f(0,0,0.01);
        dx.y = heightmap.get_value(V2f(dx.x,1-dx.z));
        dy.y = heightmap.get_value(V2f(dy.x,1-dy.z));
        dx = box_map(level_size, dx) - out.pos;
        dy = box_map(level_size, dy) - out.pos;

        V3f normal = (dx % dy);
        normal.normalize();

        out.vel = (reflect(out.vel.normalized(), normal) * 
                   out.vel.length() * 0.75);
    }

    return true;    
}

void CPUParticleSystem::draw(Camera& camera)
{
	if (particle_count < 1)
		return;

    V3f up, right;
    camera.get_billboard_axes(up, right);

    V3f ld = right * -.25 + up * -.25;
    V3f rd = right * +.25 + up * -.25;
    V3f ru = right * +.25 + up * +.25;
    V3f lu = right * -.25 + up * +.25;

    if (uv_coordinates.size() < particle_count*2*4) {
        unsigned i = uv_coordinates.size();
        uv_coordinates.resize(particle_count*2*4*2);
        positions.resize(particle_count*4*3*2);
        colors.resize(particle_count*4*4*2);

        float* f = &(uv_coordinates[i]);

        for(; i < uv_coordinates.size();i+=8) {
            *f = 0.0; ++f;
            *f = 0.0; ++f;

            *f = 1.0; ++f;
            *f = 0.0; ++f;

            *f = 1.0; ++f;
            *f = 1.0; ++f;

            *f = 0.0; ++f;
            *f = 1.0; ++f;
        }
    }


	assert(positions.size() > 0);
	assert(colors.size() > 0);

    float *p = &(positions[0]);
    float *c = &(colors[0]);

    for (unsigned i = 0; i < particle_count; ++i) {
        V3f& pos = particles[i].pos;
        Color4f& col = particles[i].color;

        *p = pos.x + ld.x; ++p;
        *p = pos.y + ld.y; ++p;
        *p = pos.z + ld.z; ++p;
        *c = col.r; ++c;
        *c = col.g; ++c;
        *c = col.b; ++c;
        *c = col.a; ++c;

        *p = pos.x + rd.x; ++p;
        *p = pos.y + rd.y; ++p;
        *p = pos.z + rd.z; ++p;
        *c = col.r; ++c;
        *c = col.g; ++c;
        *c = col.b; ++c;
        *c = col.a; ++c;

        *p = pos.x + ru.x; ++p;
        *p = pos.y + ru.y; ++p;
        *p = pos.z + ru.z; ++p;
        *c = col.r; ++c;
        *c = col.g; ++c;
        *c = col.b; ++c;
        *c = col.a; ++c;

        *p = pos.x + lu.x; ++p;
        *p = pos.y + lu.y; ++p;
        *p = pos.z + lu.z; ++p;
        *c = col.r; ++c;
        *c = col.g; ++c;
        *c = col.b; ++c;
        *c = col.a; ++c;
    }

    draw_shader.bind();

    particle_tex.bind(GL_TEXTURE0);
    draw_shader.set_uniform("texture", 0);

    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, &(colors[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(uv_coordinates[0]));
    glVertexPointer(3, GL_FLOAT, 0, &(positions[0]));

    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    glDepthMask(GL_FALSE);

    glDrawArrays(GL_QUADS, 0, particle_count*4);

    glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);
    
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);    

    draw_shader.unbind();

    particle_tex.unbind(GL_TEXTURE0);
}

TransformFeedbackParticleSystem::TransformFeedbackParticleSystem
    (GLuint max_particles, Config* config)
        : last_stat_print(glfwGetTime()), calculating(false),
          step_shader(config->resource_dir + "GLSL/step_particles",
                      config->resource_dir + "GLSL/step_particles",
                      config->resource_dir + "GLSL/step_particles",
                      GL_POINTS, GL_POINTS, 1),
          draw_shader(config->resource_dir + "GLSL/draw_particles",
                      config->resource_dir + "GLSL/draw_particles",
                      config->resource_dir + "GLSL/draw_particles",
                      GL_POINTS, GL_TRIANGLE_STRIP, 4),
          feedback(step_shader, GL_POINTS,
                   "out_pos out_color out_vel out_life",
                   max_particles, true),
          particle_tex((config->resource_dir + "textures/heart-particle.png").c_str()),
          heightmap((config->resource_dir + config->heightmap_file).c_str())
{
    heightmap.normalize();
    heightmap.send_to_GPU();

    feedback.add_draw_bindings(step_shader,
                               "gl_Vertex gl_Color vel life");
    feedback.add_draw_bindings(draw_shader,
                               "gl_Vertex gl_Color _ _");

    vel_attrib = step_shader.get_attribute_location("vel");
    life_attrib = step_shader.get_attribute_location("life");

    step_shader.bind();
    step_shader.set_uniform("water_level", config->water_level);
    step_shader.set_uniform("level_size", config->level_size.size());
    step_shader.unbind();
}

void TransformFeedbackParticleSystem::step_simulation(float time_diff)
{
    step_shader.bind();
    step_shader.set_uniform("time_diff", time_diff);

    heightmap.bind(GL_TEXTURE0);

    step_shader.set_uniform("heightmap", 0);

    feedback.begin();

    glBegin(GL_POINTS);

    Particle p;

    for (set<ParticleSource*>::iterator i = sources.begin();
         i != sources.end(); ++i) {
        while ((*i)->has_particle()) {
            (*i)->get_particle(p);

            glVertexAttrib3f(vel_attrib, p.vel.x, p.vel.y, p.vel.z);
            glVertexAttrib1f(life_attrib, p.life);
            glColor(p.color);
            glVertex(p.pos);
        }
    }
    
    glEnd();

    feedback.draw(step_shader);
    feedback.end();

    heightmap.unbind(GL_TEXTURE0);

    step_shader.unbind();

    calculating = true;


    // Statistics output

    if (glfwGetTime() - last_stat_print > 10.0) {
        last_stat_print = glfwGetTime();

        cout << feedback.get_primitive_count() << " particles." << endl;
    }
}

void TransformFeedbackParticleSystem::draw(Camera& camera)
{
    if (calculating) {
        feedback.finish();
        feedback.swap();
        calculating = false;
    }
    
    V3f up, right;
    
    camera.get_billboard_axes(up, right);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    glDepthMask(GL_FALSE);
    
    draw_shader.bind();
    
    particle_tex.bind(GL_TEXTURE0);

    draw_shader.set_uniform("up", up);
    draw_shader.set_uniform("right", right);
    draw_shader.set_uniform("texture", 0);

    feedback.draw(draw_shader);

    draw_shader.unbind();

    particle_tex.unbind(GL_TEXTURE0);


    glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);
}

void ParticleSystem::add_source(ParticleSource* source) 
{
    sources.insert(source);
}

void ParticleSystem::del_source(ParticleSource* source) 
{
    sources.erase(source);
}

StaticParticleSource::StaticParticleSource(ParticleSystem* system,
                                           V3f position,
                                           double rate, 
                                           Color4f color)
    : ParticleSource(system),
      time(0.0), rate(1.0/rate), position(position), color(color),
      rand((unsigned long)(100000*glfwGetTime())){}

bool StaticParticleSource::has_particle()
{
    return (time > rate);
}

void StaticParticleSource::get_particle(Particle& p) {
    time -= rate;

    p.pos = position + Imath::solidSphereRand<V3f, Rand32>(rand) * 10;
    V3f c = hsv2rgb(V3f(rand.nextf(0.75,0.95),
                        rand.nextf(0.8,1.0),
                        rand.nextf(0.4,0.6)));
    p.color = Color4f(c.x,c.y,c.z,1);
    p.life = 10.0f + rand.nextf(0.0,20.0);
    
    p.vel = V3f(0,0,0);

    p.vel = (Imath::hollowSphereRand<V3f, Rand32>(rand)  
             * (fabs(Imath::gaussRand(rand)) + 2)
             * 50);


} 

PonyParticleSource::PonyParticleSource(ParticleSystem* system)
    : ParticleSource(system),
      time(0.0), rate(1.0/rate),
      pos(0,0,0), dir(0,1,0), color(1,0,0,1),
      explosion_particles(0),
      rand((unsigned long)(100000*glfwGetTime())){}

bool PonyParticleSource::has_particle()
{
    return (explosion_particles > 0 || 
            (rate > 0.0 && time > rate));
}

void PonyParticleSource::get_particle(Particle& p) {
    if (explosion_particles > 0) {
        --explosion_particles;
        p.pos = pos - V3f(0,4,0) + Imath::solidSphereRand<V3f, Rand32>(rand) * 2;
        V3f hsv = rgb2hsv(V3f(color.r,color.g,color.b));
        
        V3f c   = hsv2rgb(V3f(frac(hsv.x + gaussRand(rand) / 8.0),
                              1.0,
                              rand.nextf(0.4,0.6)));
        p.color = Color4f(c.x,c.y,c.z,1);
        p.life = 10.0f + rand.nextf(0.0,20.0);
        
        p.vel = V3f(0,0,0);
        
        p.vel = (Imath::hollowSphereRand<V3f, Rand32>(rand)  
                 * (fabs(Imath::gaussRand(rand)) + 2)
                 * 10);

    } else if (rate > 0.0) {
        time -= rate;
        
        V3f offset = Imath::solidSphereRand<V3f, Rand32>(rand) * 3;

        p.pos   = pos + offset;   
        V3f hsv = rgb2hsv(V3f(color.r,color.g,color.b));
        
        V3f c   = hsv2rgb(V3f(frac(hsv.x + gaussRand(rand) / 8.0),
                              1.0,
                              rand.nextf(0.4,0.6)));
        p.color = Color4f(c.x,c.y,c.z,1);
        p.life  = 10.0f + rand.nextf(0.0,20.0);
    
        p.vel   = -dir * 10 + offset * 1;

    }
}

void PonyParticleSource::set_pos(V3f pos) {
    this->pos = pos;
}

void PonyParticleSource::set_dir(V3f dir) {
    this->dir = dir;
}

void PonyParticleSource::set_color(Color4f color) {
    this->color = color;
}

void PonyParticleSource::set_rate(double rate) {
    if (rate > 0.0) {
        this->rate = 1.0/rate;
    } else {
        this->rate = -1.0;
    }
}

void PonyParticleSource::explode(int particles) {
    explosion_particles += particles;
}

ParticleExplosionSource::ParticleExplosionSource(ParticleSystem* system)
    : ParticleSource(system),
      rand((unsigned long)(100000*glfwGetTime())){};

bool ParticleExplosionSource::has_particle()
{
    return explosions.size() > 0;
}

void ParticleExplosionSource::get_particle(Particle& p)
{
    int s = explosions.size() - 1;

    if (s >= 0) {
        V3f&     pos   = explosions[s].pos;
        Color4f& color = explosions[s].color;
        int&     count = explosions[s].count;
        float&   var   = explosions[s].var;


        p.pos = pos - V3f(0,4,0) + Imath::solidSphereRand<V3f, Rand32>(rand) * 2;
        V3f hsv = rgb2hsv(V3f(color.r,color.g,color.b));
        
        V3f c   = hsv2rgb(V3f(frac(hsv.x + gaussRand(rand) * var),
                              1.0,
                              rand.nextf(0.4,0.6)));
        p.color = Color4f(c.x,c.y,c.z,1);
        p.life = 10.0f + rand.nextf(0.0,20.0);
        
        p.vel = V3f(0,0,0);
        
        p.vel = (Imath::hollowSphereRand<V3f, Rand32>(rand)  
                 * (fabs(Imath::gaussRand(rand)) + 2)
                 * 10);

        
        --count;
        if (count < 1) {
            explosions.resize(s);
        }
    }
}

void ParticleExplosionSource::explode(V3f pos,
                                      Color4f color,
                                      int count,
                                      float var)
{
    explosions.push_back(Explosion(pos, color, count, var));
}
