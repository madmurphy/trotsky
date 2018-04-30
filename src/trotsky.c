/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * trotsky.c
 * Copyright (C) 2018 madmurphy <madmurphy333@gmail.com>
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
#define TROTSKY_UI "src/trotsky.ui"
#else
#define TROTSKY_UI PACKAGE_DATA_DIR"/ui/trotsky.ui"
#endif

/* What follows is the list of the widgets contained in TROTSKY_UI
that will be accessible to the code through "trotsky_widgets.[WidgetID]".
Whatever is present in this list will be present in the structure
"trotsky_widgets" as well. The format of each row is:

	_____([WidgetType],	[WidgetCastFunction], [WidgetID]);	\

Please, make sure that each [WidgetID] is contained in trotsky.ui! */
#define TROTSKY_WIDGETS_LIST(_____)							\
															\
	_____(GtkWidget,	GTK_WIDGET,		trotsky_window)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_1)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_2)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_3)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_4)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_5)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_6)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_7)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_8)		\
	_____(GtkEntry,		GTK_ENTRY,		my_text_box_9)		\
	_____(GtkSwitch,	GTK_SWITCH,		my_switch_box_1)

/* Private definitions for `trotsky_widgets` */
#define _AS_DECLARATION_(_W_TYPE_, _W_CAST_, _W_ID_) _W_TYPE_ * _W_ID_;
#define TROTSKY_GTK_WIDGETS struct trotsky_widgets_t { TROTSKY_WIDGETS_LIST(_AS_DECLARATION_) }

/* The struct `trotsky_widgets` is the container of all the widgets that we need */
TROTSKY_GTK_WIDGETS trotsky_widgets;

#undef TROTSKY_GTK_WIDGETS
#undef _AS_DECLARATION_

/* Signal handlers */
/* Note: These may not be declared static because signal autoconnection
 * only works with non-static methods
 */

/* Called when the window is closed */
void on_window_destroy (GtkWidget *widget, gpointer data) {
	gtk_main_quit ();
}

static void init_UI () {

	GtkBuilder *trotsky_glade;
	GError* error = NULL;

	/* Load UI from file */
	trotsky_glade = gtk_builder_new ();

	if (!gtk_builder_add_from_file(trotsky_glade, TROTSKY_UI, &error)) {
		g_critical("Couldn't load builder file: %s", error->message);
		g_error_free(error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (trotsky_glade, NULL);

	/* Populate the trotsky_widgets struct */
	#define _BUILDER_OBJ_ trotsky_glade
	#define _AS_ASSIGNEMENT_(_W_TYPE_, _W_CAST_, _W_ID_) \
		trotsky_widgets._W_ID_ = _W_CAST_(gtk_builder_get_object(_BUILDER_OBJ_, #_W_ID_));\
		if (!trotsky_widgets._W_ID_) { g_critical ("Widget" #_W_ID_ " is missing in file %s.", TROTSKY_UI); }

	TROTSKY_WIDGETS_LIST(_AS_ASSIGNEMENT_);

	#undef _AS_ASSIGNEMENT_
	#undef _BUILDER_OBJ_

	g_object_unref(trotsky_glade);

	/* Start the application... */

	gtk_widget_show(trotsky_widgets.trotsky_window);

}

int main (int argc, char *argv[]) {

	gtk_init (&argc, &argv);

	init_UI();

	gtk_entry_set_text(trotsky_widgets.my_text_box_1, "Hello world!");
	gtk_entry_set_text(trotsky_widgets.my_text_box_2, "Foo");
	gtk_entry_set_text(trotsky_widgets.my_text_box_3, "Bar");
	gtk_entry_set_text(trotsky_widgets.my_text_box_4, "Blablabla");
	gtk_entry_set_text(trotsky_widgets.my_text_box_5, "Some text here");
	gtk_entry_set_text(trotsky_widgets.my_text_box_6, "Some other text here");
	gtk_entry_set_text(trotsky_widgets.my_text_box_7, "...and so on...");
	gtk_entry_set_text(trotsky_widgets.my_text_box_8, "It's just a simple way to keep track of the widgets...");
	gtk_entry_set_text(trotsky_widgets.my_text_box_9, "...through the preprocessor!");
	gtk_switch_set_state(trotsky_widgets.my_switch_box_1, TRUE);

	gtk_main();

	return 0;

}

#undef TROTSKY_WIDGETS_LIST

/* EOF */


