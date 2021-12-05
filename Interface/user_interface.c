#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "grayscale2.h"
#include "pixel_operations.h"
#include "segmentation.h"
#include <math.h>
#include <SDL/SDL_rotozoom.h>

typedef struct UserInterface
{
  GtkWindow* window;             // Main window
  GtkButton* open_button;        // Open button
  GtkWidget* quit_button;        // Quit button
  GtkWidget* resolve_button;     //Resolving the sudoku
  GtkBuilder* builder;           //just to save the builder
  GtkWidget* file_dialog;        //widget to choose a file to open
  GtkWidget* next_button;
} UserInterface;

GtkBuilder* builder;
GtkWidget* image;
GtkWidget* image_sol;

GtkWidget* paneau;

SDL_Surface *img;
SDL_Surface *img2;
int img2i;

char* filename;
int countnext = 0;
int done = 0;
int intertout = 1;
int res;
int i = 0;
int j = 0;
FILE *fptr;

int e[9][9] = {0};
char txte[81];


void on_next_button_clicked()
{ 
    //UserInterface* i = ui; 
    switch(countnext) 
    {
        case 0:
            //CAS GRAYSCALE
            //
            //
                if(!done)
            {
            img = IMG_Load(filename);
  
            SDL_SaveBMP(img, "cache/s");

            
            gtk_container_remove(GTK_CONTAINER(paneau), image);

            img2i = pretraitement(img,1);

            done = 1;
     

            } 

            

            countnext++;
            break;
        case 1:
            //CAS VIDE
            //

            image = gtk_image_new_from_file("image3.bmp");
            filename = "image3.bmp";

            GdkPixbuf* pixbuf2 = gdk_pixbuf_new_from_file_at_scale
                (filename,801,801,TRUE, NULL);
            gtk_image_set_from_pixbuf(GTK_IMAGE(image),pixbuf2);

            gtk_container_add(GTK_CONTAINER(paneau),image);
            

            gtk_widget_show(image);


            countnext++;
            break;
        case 2:

            //itertout
            while(intertout < 82)
            {
               

                res = NicoMartin(img,intertout);
                g_print("%d\n",intertout);
                //écrire dans un fichier .txt
                txte[intertout-1] = res;
                /*
                j++;
                if(j > 8)
                {
                    i++;
                    j = 0;
                }*/
                intertout++;                
            }

                        
            i = 0;
            j = 0;
            fptr = fopen("sortieneuron","w+");
            /*
            for(int i = 0; i < 9; i++)
            {
                g_print("AAAAAA");
                for(int j = 0; j < 9; j++)
                {
                    g_print("o");
                    fprintf(fptr,"%d", e[i][j]);
                }
                fputs("\n",fptr);
            }*/
            while(i<81)
            {
                fprintf(fptr,"%d",txte[i]);
                i++;
            }
            
            countnext++;
            break;
            
        case 3:
            //CAS VIDE
            //
            //

            image = gtk_image_new_from_file("logo.png");
            filename = "logo.png";
           
            gtk_container_remove(GTK_CONTAINER(paneau), image);

            gtk_widget_show(image);
            
            countnext++;
            break;
        default:
            //save the sudoku

            countnext++;
            break;
    }
}


// Signal handler for the "clicked" signal of the buttons.
void on_open(GtkWidget *button, gpointer ui)
{
    UserInterface* i = ui;
    (void)button;
    GtkWidget* dialog = i->file_dialog;
    image = GTK_WIDGET(gtk_builder_get_object(builder,"image_print"));
    switch(gtk_dialog_run(GTK_DIALOG(dialog)))
      {
        case GTK_RESPONSE_ACCEPT:
          {
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            //gtk_image_set_from_file(GTK_IMAGE(i->image),filename);
            /*
            int width = gtk_widget_get_allocated_width(taillefenetre); 
            int height = gtk_widget_get_allocated_height(taillefenetre);*/
            GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_scale
                (filename,801,801,TRUE, NULL);
            gtk_image_set_from_pixbuf(GTK_IMAGE(image),pixbuf);

            gtk_widget_show(image);

            //init_sdl();
            //i->image_surface = load_image(filename);

            gtk_widget_set_visible(GTK_WIDGET(i->open_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->open_button),TRUE);
            
            gtk_widget_set_visible(GTK_WIDGET(i->quit_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->quit_button),TRUE);

            gtk_widget_set_visible(GTK_WIDGET(i->next_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->next_button),TRUE);


            gtk_widget_set_visible(GTK_WIDGET(i->resolve_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->resolve_button),TRUE);

            gtk_button_set_label(i->open_button, "Open a New File");
            g_print(filename);

            break;
          }
        default:
          {
            break;
          }
      }
    gtk_widget_hide(dialog);
}

void on_resolve(GtkWidget *button,gpointer ui)
{
    UserInterface* i = ui;
    (void)button;
    gtk_widget_set_visible(GTK_WIDGET(i->resolve_button),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(i->resolve_button),FALSE);
    
    //resize the image 1000x1000
    
    //put in FS the window
    gtk_window_maximize(GTK_WINDOW(i->window));
}

// Main function.
int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);
    //Init_nn();

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    builder = gtk_builder_new_from_file("user_interface.glade");    

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object
            (builder, "org.gtk.OCR"));

    GdkRGBA color;
    color.red = 000000;
    color.green = 000000;
    color.blue = 000000;
    gtk_widget_override_background_color(GTK_WIDGET(window),
            GTK_STATE_NORMAL, &color);

    GtkButton* open_button = GTK_BUTTON
        (gtk_builder_get_object(builder, "open_button"));
    GtkWidget* resolve_button = GTK_WIDGET
        (gtk_builder_get_object(builder, "resolve_button"));
    GtkWidget* quit_button = GTK_WIDGET
        (gtk_builder_get_object(builder, "quit_button"));
    GtkWidget* next_button = GTK_WIDGET
        (gtk_builder_get_object(builder, "next_button"));
    paneau = GTK_WIDGET(gtk_builder_get_object(builder, "paneau"));
    

    mkdir("cache",0777); 

    GtkFileFilter* filter = gtk_file_filter_new();
    GtkWidget* file_dialog = gtk_file_chooser_dialog_new
        ("Open image",window,GTK_FILE_CHOOSER_ACTION_OPEN,"Open",
         GTK_RESPONSE_ACCEPT,"Cancel",GTK_RESPONSE_CANCEL,NULL);
    
/*
    GdkPixbuf* pixbuf2 = gdk_pixbuf_new_from_file_at_scale
    ("image_solve.png",801,801,TRUE, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_solve.png),pixbuf2);
*/

    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog),filter); 
    
    UserInterface ui=
      {
       .window = window,
       .open_button = open_button,
       .quit_button = quit_button,
       .resolve_button = resolve_button,
       .file_dialog = file_dialog,
       .builder = builder,
       .next_button = next_button,
      };

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open), &ui);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(resolve_button, "clicked", G_CALLBACK(on_resolve), &ui);
    g_signal_connect(next_button,"clicked",
            G_CALLBACK(on_next_button_clicked),NULL);
    //
    gtk_builder_connect_signals(builder,NULL);

       // Runs the main loop.    
    gtk_main();

    // Exits.
    return 0;
}
