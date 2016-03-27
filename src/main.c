/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2016 John Doe <example@example.org>
 * 
 * trotsky is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * trotsky is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <gtk/gtk.h>

/* For testing purpose, define TEST to use the local (not installed) ui file */
#define TEST
#ifdef TEST
#define UI_FILE "src/trotsky.ui"
#else
#define UI_FILE PACKAGE_DATA_DIR"/ui/trotsky.ui"
#endif
#define TOP_WINDOW "window"

/* What follows is the list of the widgets contained in UI_FILE
that will be accessible to the code through "app_widgets.[WidgetID]".
Whatever is present in this list will be present in the structure
"app_widgets" as well. The format of each row is:

	__([WidgetType], [WidgetCastFunction], [WidgetID]); \

Please, make sure that each [WidgetID] is contained in UI_FILE! */
#define TROTSKY_WIDGETS_LIST(__) \
	__(GtkWidget, GTK_WIDGET, trotsky_window);\
	__(GtkEntry, GTK_ENTRY, my_text_box_1);\
	__(GtkEntry, GTK_ENTRY, my_text_box_2);\
	__(GtkEntry, GTK_ENTRY, my_text_box_3);\
	__(GtkEntry, GTK_ENTRY, my_text_box_4);\
	__(GtkEntry, GTK_ENTRY, my_text_box_5);\
	__(GtkEntry, GTK_ENTRY, my_text_box_6);\
	__(GtkEntry, GTK_ENTRY, my_text_box_7);\
	__(GtkEntry, GTK_ENTRY, my_text_box_8);\
	__(GtkEntry, GTK_ENTRY, my_text_box_9);\
	__(GtkSwitch, GTK_SWITCH, my_switch_box_1);

/* Private definitions reserved for TROTSKY_WIDGETS_LIST */
#define _AS_DECLARATION_(_w_type_, _w_cast_, _w_id_) _w_type_ * _w_id_
#define _AS_ASSIGNEMENT_(_w_type_, _w_cast_, _w_id_) app_widgets._w_id_ = _w_cast_(gtk_builder_get_object(builder, #_w_id_)); trotsky_widget_status(!app_widgets._w_id_, #_w_id_, UI_FILE);
#define TrotskyGtkWidgets struct app_widgets_t { TROTSKY_WIDGETS_LIST(_AS_DECLARATION_) }
#define TROTSKY_CONFIGURE_WIDGETS() TROTSKY_WIDGETS_LIST(_AS_ASSIGNEMENT_);

/* The struct app_widgets is the container of all the widgets that we need */
TrotskyGtkWidgets app_widgets;

static void trotsky_widget_status (gboolean w_is_missing, char * w_name, char * ui_file) {
	if (w_is_missing) {
		g_critical("Widget \"%s\" is missing in file %s.", w_name, ui_file);
	}
}

/* Signal handlers */
/* Note: These may not be declared static because signal autoconnection
 * only works with non-static methods
 */

/* Called when the window is closed */
void on_window_destroy (GtkWidget *widget, gpointer data) {
	gtk_main_quit ();
}

static void init_UI () {

	GtkBuilder *builder;
	GError* error = NULL;

	/* Load UI from file */
	builder = gtk_builder_new ();

	if (!gtk_builder_add_from_file (builder, UI_FILE, &error)) {
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, NULL);

	/* Populate the app_widgets struct */

	TROTSKY_CONFIGURE_WIDGETS();

	g_object_unref(builder);

	/* Start the application... */

	gtk_widget_show(app_widgets.trotsky_window);

}

int main (int argc, char *argv[]) {

	gtk_init (&argc, &argv);

	init_UI();

	gtk_entry_set_text(app_widgets.my_text_box_1, "Hello world!");
	gtk_entry_set_text(app_widgets.my_text_box_2, "Foo");
	gtk_entry_set_text(app_widgets.my_text_box_3, "Bar");
	gtk_entry_set_text(app_widgets.my_text_box_4, "Blablabla");
	gtk_entry_set_text(app_widgets.my_text_box_5, "Some text here");
	gtk_entry_set_text(app_widgets.my_text_box_6, "Some other text here");
	gtk_entry_set_text(app_widgets.my_text_box_7, "...and so on...");
	gtk_entry_set_text(app_widgets.my_text_box_8, "...It's just a simple way to keep track of the widgets...");
	gtk_entry_set_text(app_widgets.my_text_box_9, "...through the preprocessor!");
	gtk_switch_set_state(app_widgets.my_switch_box_1, TRUE);

	gtk_main();

	return 0;

}

