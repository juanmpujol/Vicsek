# include <cmath>
# include <cstdlib>
# include <fstream>
# include <random>
# include <iostream> 
# include <vector> 
# include <SDL.h>
#include <SDL_ttf.h>

constexpr auto L = 300;
constexpr auto PI = 3.1415926;
constexpr auto STEPS = 2000;
constexpr auto LINE_LEN = 5.0;




class ParticleClass
{
public:
    double xpos, ypos, angle;

};


bool distance_in_range(ParticleClass mainParticle, ParticleClass nbrParticle, float range);


void update_particles(ParticleClass* particles, float noise, float range, float vel, int N);





int main(int argc, char* args[]) {

    int N;
    double noise, range, vel;




    std::random_device rd;
    std::mt19937 e{ rd() };
    
    std::cout << "Nro de particulas: \n";
    std::cin >> N;

    std::cout << "Ruido: \n";
    std::cin >> noise;

    std::cout << "Rango de acci�n (entre 0 y 300): \n";
    std::cin >> range;

    std::cout << "Velocidad: \n";
    std::cin >> vel;


    ParticleClass* particles = new ParticleClass[sizeof(ParticleClass) * N];

    // creo un array de elementos de la clase de part�culas asign�ndole la memoria de tama�o necesario para almacenar los N elementos



    // Inicializo el rng para setear coordenadas iniciales


    for (int i = 0; i < N; i++)
    {
        std::uniform_real_distribution<double> dist{0,L};
        particles[i].xpos = dist(e);
        particles[i].ypos = dist(e);
        particles[i].angle = dist(e) / L * 2 * PI;
    }

    // Inicializo y creo ventana
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("Vicsek",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        L, L+50,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;
    }

    // Inicializo el renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Inicializo el evento

    SDL_Event event;
    int running = 1;
    unsigned int lastTicks = SDL_GetTicks();
    double t = 0;
    int counter = 0;


    while (running)
    {

        unsigned int ticks = SDL_GetTicks();
        float dt = (ticks - lastTicks) / 1000.0f;
        lastTicks = ticks;

        // Actualizo las part�culas

        update_particles(particles, noise, range, vel, N);

        t += dt;

        unsigned int endUpdateTicks = SDL_GetTicks();

        // Limpio la pantalla anterior y dibujo las part�culas
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);

        for (int i = 0; i < N; i++)
        {


            SDL_RenderDrawLine(renderer,
                particles[i].xpos,
                particles[i].ypos,
                particles[i].xpos + LINE_LEN * cos(particles[i].angle),
                particles[i].ypos + LINE_LEN * sin(particles[i].angle));
            
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(0);

        counter++;

        if (counter == 500) {
            break;
        }

    }


    return 0;

}



bool distance_in_range(ParticleClass mainParticle, ParticleClass nbrParticle, float range)
{
    double dx, dy;
    dx = nbrParticle.xpos - mainParticle.xpos;
    dy = nbrParticle.ypos - mainParticle.ypos;
    if (dx * dx + dy * dy <= range) return true;
    return false;
};


void update_particles(ParticleClass* particles, float noise, float range, float vel, int N)
{

    for (int i = 0; i < N; i++)
    {
        ParticleClass mainParticle = particles[i];
        int counter = 0;
        float nbrAngle = 0;

        for (int j = 0; j < N; j++)
        {
            ParticleClass nbrParticle = particles[j];

            // para cada part�cula en rango de acci�n actualizo el �ngulo

            if (distance_in_range(mainParticle, nbrParticle, range))
            {
                nbrAngle += sin(nbrParticle.angle - mainParticle.angle);
                counter += 1;
            }

        }

        mainParticle.xpos += vel * cos(mainParticle.angle);
        mainParticle.ypos += vel * sin(mainParticle.angle);

        // actualizo las posiciones, agregando condiciones peri�dicas de contorno

        if (mainParticle.xpos < 0) mainParticle.xpos += L;
        if (mainParticle.xpos > L) mainParticle.xpos -= L;
        if (mainParticle.ypos < 0) mainParticle.ypos += L;
        if (mainParticle.ypos > L) mainParticle.ypos -= L;


        if (counter > 0)
        {
            mainParticle.angle += nbrAngle / counter + noise * ((float)rand() / (float)RAND_MAX * 2 * PI - PI);
        }

        // actualizo el �ngulo despu�s de que la part�cula se haya movido
        // le agrego como ruido un nro aleatorio entre -pi, pi y lo multiplico por una "amplitud"

        particles[i] = mainParticle;

        // escribo los nuevos valores de la part�cula  al array original
    }
};

