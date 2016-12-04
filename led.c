#define GPIO_BASE 0x20200000
#define BLOCK_SIZE (4*1024)

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int  mem_fd;
volatile int *gpio_map;


void setup_io()
{
   /* On ouvre le fichier /dev/mem qui permet de mapper toute la mémoire */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /* On demande au noyau de nous donner un mapping virtuel à l'adresse GPIO_BASE */
      gpio_map = (unsigned int *)mmap(
      NULL,             // Toute adresse de map convient
      BLOCK_SIZE,       // Taille des blocks
      PROT_READ|PROT_WRITE, // En lecture et ecriture
      MAP_SHARED,       //Mode partage
      mem_fd,           //Fichier a mapper
      GPIO_BASE         //Offset du peripherique GPIO
   );

   close(mem_fd); // On peut maintenant fermer /dev/mem

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);
      exit(-1);
   }
}

int main(){
   setup_io();
   main_asm(gpio_map);
   return 0;
}
