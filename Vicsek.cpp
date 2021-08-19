# include <cmath>
# include <cstdlib>
# include <fstream>
# include <random>
# include <iostream> 

#define PI 3.1415926
#define RELSTEPS 500
#define MEASURESTEPS 2500

class ParticleClass
{
    public:
        float xpos, ypos, angle;    

};


bool distance_in_range(ParticleClass mainParticle, ParticleClass nbrParticle, float range);

void update_particles(ParticleClass* particles, float noise, float range, float vel);

void simulate_vicsek(float range, float vel, float noise, int N, int L);

void simulate_multiple(float vel, int N, int L);



int main(){

    int N, L;
    float vel; 



    vel = 0.5;
    N = 1000;

    simulate_multiple(vel, N, 300);
    simulate_multiple(vel, N, 141.42);
    simulate_multiple(vel, N, 115.47);
    simulate_multiple(vel, N, 100);
    simulate_multiple(vel, N, 77);
    simulate_multiple(vel, N, 64);
    simulate_multiple(vel, N, 50);
    simulate_multiple(vel, N, 37);
    simulate_multiple(vel, N, 31);
    simulate_multiple(vel, N, 25);
    simulate_multiple(vel, N, 20);
    simulate_multiple(vel, N, 14);
    simulate_multiple(vel, N, 10);


    
}



bool distance_in_range(ParticleClass mainParticle, ParticleClass nbrParticle, float range)
{   
    double dx, dy;
    dx = nbrParticle.xpos - mainParticle.xpos;
    dy = nbrParticle.ypos - mainParticle.ypos;
    if(dx*dx + dy*dy <= range) return true;
    return false;
};


void update_particles(ParticleClass* particles, float noise, float range, float vel, int N, int L)
{

    for (int i = 0; i < N; i++ )
    {
        ParticleClass mainParticle = particles[i];
        int counter = 0;
        float nbrAngle = 0;

        for (int j = 0; j < N; j++)
        {
            ParticleClass nbrParticle = particles[j];

            // para cada partícula en rango de acción actualizo el ángulo

            if(distance_in_range(mainParticle, nbrParticle, range))
            {
                nbrAngle += sin(nbrParticle.angle - mainParticle.angle);
                counter += 1;
            }

        }

        mainParticle.xpos += vel * cos(mainParticle.angle);
        mainParticle.ypos += vel * sin(mainParticle.angle); 

        // actualizo las posiciones, agregando condiciones periódicas de contorno

        if (mainParticle.xpos < 0) mainParticle.xpos += L;
        if (mainParticle.xpos > L) mainParticle.xpos -= L;
        if (mainParticle.ypos < 0) mainParticle.ypos += L;
        if (mainParticle.ypos > L) mainParticle.ypos -= L;


        if(counter > 0)
        {
            mainParticle.angle += nbrAngle/counter + noise*((float)rand()/(float)RAND_MAX * 2 * PI - PI);
        }

        // actualizo el ángulo después de que la partícula se haya movido
        // le agrego como ruido un nro aleatorio entre -pi, pi y lo multiplico por una "amplitud"

        particles[i] = mainParticle;

        // escribo los nuevos valores de la partícula  al array original
    }
};


void simulate_vicsek(float range, float vel, float noise, int N, int L)
{
    ParticleClass* particles = new ParticleClass[sizeof(ParticleClass)*N];

    // creo un array de elementos de la clase de partículas asignándole la memoria de tamaño necesario para almacenar los N elementos de la clase

    std::random_device rd;
    std::mt19937 e{rd()};

    // inicializo el rng para setear coordenadas iniciales


    for (int i = 0; i < N; i++)
    {        
        std::uniform_real_distribution<double> dist{0,(double)L};
        particles[i].xpos = dist(e);
        particles[i].ypos = dist(e);
        particles[i].angle = dist(e)/L * 2*PI;
    }


    float density = (float)N/(float(L*L));


    for (int t = 0; t < RELSTEPS; t++)
    {
        // para cada paso temporal llamo a la función que me actualiza las propiedaes de todas las partículas
        // estos son para que el sistema relaje, después empiezo a medir

        update_particles(particles, noise, range, vel, N, L);

        
    }




    float phix, phiy, meanPolar, meanPolarCuad, susc;
    susc = 0;

    meanPolarCuad = 0;
    meanPolar = 0;
    
    // estas son las corridas en las que "mido", asumiendo que el sistema ya relajó 

    for (int t = 0; t < MEASURESTEPS; t++ )
    {
        phix = 0;
        phiy = 0;
        
        update_particles(particles, noise, range, vel, N, L);

        for(int i = 0; i < N; i++)
        {
            phix += cos(particles[i].angle)/N;
            phiy += sin(particles[i].angle)/N;
        }

        meanPolar += sqrt(phix*phix + phiy*phiy)/MEASURESTEPS;
        meanPolarCuad += (phix*phix + phiy*phiy)/MEASURESTEPS;

    }

    susc = (meanPolarCuad - meanPolar*meanPolar)*L*L;


    std::ofstream simFile;
    simFile.open("simulacion_densidad=" + std::to_string(density) +".txt", std::ios_base::app);
    simFile << noise << ", " << susc << ", " << meanPolar << "\n";
    simFile.close();
    std::cout << "termina una sim \n" ; 
};


void simulate_multiple(float vel, int N, int L)
{
    simulate_vicsek(0.25*L, 3, 0.05, N, L);
    simulate_vicsek(0.25*L, 3, 0.075, N, L);
    simulate_vicsek(0.25*L, 3, 0.1, N, L);
    simulate_vicsek(0.25*L, 3, 0.15, N, L);
    simulate_vicsek(0.25*L, 3, 0.2, N, L);
    simulate_vicsek(0.25*L, 3, 0.25, N, L);
    simulate_vicsek(0.25*L, 3, 0.275, N, L);
    simulate_vicsek(0.25*L, 3, 0.3, N, L);
    simulate_vicsek(0.25*L, 3, 0.32, N, L);
    simulate_vicsek(0.25*L, 3, 0.33, N, L);
    simulate_vicsek(0.25*L, 3, 0.34, N, L);
    simulate_vicsek(0.25*L, 3, 0.35, N, L);
    simulate_vicsek(0.25*L, 3, 0.36, N, L);
    simulate_vicsek(0.25*L, 3, 0.37, N, L);
    simulate_vicsek(0.25*L, 3, 0.38, N, L);
    simulate_vicsek(0.25*L, 3, 0.4, N, L);
    simulate_vicsek(0.25*L, 3, 0.42, N, L);
    simulate_vicsek(0.25*L, 3, 0.43, N, L);
    simulate_vicsek(0.25*L, 3, 0.44, N, L);
    simulate_vicsek(0.25*L, 3, 0.445, N, L);
    simulate_vicsek(0.25*L, 3, 0.45, N, L);
    simulate_vicsek(0.25*L, 3, 0.455, N, L);
    simulate_vicsek(0.25*L, 3, 0.46, N, L);
    simulate_vicsek(0.25*L, 3, 0.465, N, L);
    simulate_vicsek(0.25*L, 3, 0.47, N, L);
    simulate_vicsek(0.25*L, 3, 0.475, N, L);
    simulate_vicsek(0.25*L, 3, 0.48, N, L);
    simulate_vicsek(0.25*L, 3, 0.5, N, L);
    simulate_vicsek(0.25*L, 3, 0.52, N, L);
    simulate_vicsek(0.25*L, 3, 0.54, N, L);
    simulate_vicsek(0.25*L, 3, 0.55, N, L);
    simulate_vicsek(0.25*L, 3, 0.56, N, L);
    simulate_vicsek(0.25*L, 3, 0.58, N, L);
    simulate_vicsek(0.25*L, 3, 0.6, N, L);        
    simulate_vicsek(0.25*L, 3, 0.625, N, L);
    simulate_vicsek(0.25*L, 3, 0.65, N, L);
    simulate_vicsek(0.25*L, 3, 0.7, N, L);
    simulate_vicsek(0.25*L, 3, 0.8, N, L);
    simulate_vicsek(0.25*L, 3, 1.0, N, L);





    std::cout << "termina para L=" << L << "\n";

}
