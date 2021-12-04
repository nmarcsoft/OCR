#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct UserInterface
{
  GtkWindow* window;             // Main window
  GtkWidget* open_button;        // Open button
  //GtkWidget* close_button;       // Close button
  GtkWidget* quit_button;        // Quit button
  GtkWidget* resolve_button;     //Resolving the sudoku
  GtkBuilder* builder;           //just to save the builder
  GtkWidget* file_dialog;        //widget to choose a file to open
  GtkWidget* image;              //the image gtk will display, it is the same that we will apply ocr on
} UserInterface;

/*
GtkWindow* window;             // Main window
GtkWidget* open_button;        // Open button
GtkWidget* close_button;       // Close button
GtkWidget* quit_button;        // Quit button
GtkWidget* resolve_button;     //Resolving the sudoku
GtkBuilder* builder;           //just to save the builder
GtkWidget* file_dialog;        //widget to choose a file to open
GtkWidget* image;              //the image gtk will display, it is the same that we will apply ocr on


GtkWidget *page0;*/


// Signal handler for the "clicked" signal of the buttons.
void on_open(GtkWidget *button, gpointer ui)
{
    UserInterface* i = ui;
    (void)button;
    GtkWidget* dialog = i->file_dialog;
    i->image = GTK_WIDGET(gtk_builder_get_object(i->builder,"image_print"));
    switch(gtk_dialog_run(GTK_DIALOG(dialog)))
      {
        case GTK_RESPONSE_ACCEPT:
          {
            gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            gtk_image_set_from_file(GTK_IMAGE(i->image),filename);
            gtk_widget_show(i->image);

            //init_sdl();
            //i->image_surface = load_image(filename);

            gtk_widget_set_visible(GTK_WIDGET(i->open_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->open_button),TRUE);

            /*
            gtk_widget_set_visible(GTK_WIDGET(i->close_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->close_button),TRUE);
            */
            
            gtk_widget_set_visible(GTK_WIDGET(i->quit_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->quit_button),TRUE);


            gtk_widget_set_visible(GTK_WIDGET(i->resolve_button),TRUE);
            gtk_widget_set_sensitive(GTK_WIDGET(i->resolve_button),TRUE);

            //gtk_button_set_label(i->open_button, "Open a New File");

            break;
          }
        default:
          {
            break;
          }
      }
    gtk_widget_hide(dialog);
}
/*
void on_close(GtkButton *button,gpointer ui)
{
  UserInterface* i = ui;
  (void)button;

  gtk_widget_hide(i->image);
  //SDL_FreeSurface(i->image_surface);

  gtk_widget_set_visible(GTK_WIDGET(i->close_button),FALSE);

  gtk_widget_set_sensitive(GTK_WIDGET(i->close_button),FALSE);

  gtk_widget_set_visible(GTK_WIDGET(i->open_button),TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(i->open_button),TRUE);


  gtk_widget_set_visible(GTK_WIDGET(i->quit_button),TRUE);
  gtk_widget_set_sensitive(GTK_WIDGET(i->quit_button),TRUE);

  gtk_widget_set_visible(GTK_WIDGET(i->resolve_button),FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(i->resolve_button),FALSE);
}

// Event handler for the "clicked" signal of the resolve button
void on_resolve(GtkButton *button, gpointer ui)
{
    g_print("on_resolve()\n");
}*/

// Main function.
int main()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);
    //Init_nn();

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "user_interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.OCR"));


    GtkWidget* open_button = GTK_WIDGET(gtk_builder_get_object(builder, "open_button"));
    //close_button = GTK_WIDGET(gtk_builder_get_object(builder, "close_button"));
    GtkWidget* resolve_button = GTK_WIDGET(gtk_builder_get_object(builder, "resolve_button"));
    GtkWidget* quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "quit_button"));
    
    //page0 = GTK_WIDGET(gtk_builder_get_object(builder, "page0"));

    GtkFileFilter* filter = gtk_file_filter_new();
    GtkWidget* file_dialog = gtk_file_chooser_dialog_new("Open image",window,GTK_FILE_CHOOSER_ACTION_OPEN,"Open",GTK_RESPONSE_ACCEPT,"Cancel",GTK_RESPONSE_CANCEL,NULL);
    
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_dialog),filter);
    
    
    
    UserInterface ui=
      {
       .window = window,
       .open_button = open_button,
       //.close_button = close_button,
       .quit_button = quit_button,
       .resolve_button = resolve_button,
       .file_dialog = file_dialog,
       .builder = builder,
      };

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open), &ui);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(resolve_button, "clicked", G_CALLBACK(on_resolve), &ui);
    // Runs the main loop.    
    gtk_main();

    // Exits.
    return 0;
}
