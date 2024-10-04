
#include "ligador.h"

Vector find_all_identifier_pos( program_t *p, token_t *tok )
{
    Vector *rt = malloc(sizeof(Vector));
    initVector( rt, 5 );

    int pc = 0;
    for ( int i=0; i < p->n_tokens; i++ )
    {
        token_t t = p->tokens[i];
        switch ( t.type ) 
        {
                case TOK_STORE:
                case TOK_LOAD:
                case TOK_ADD:
                case TOK_SUB:
                case TOK_BRNEG:
                case TOK_BRZERO:
                case TOK_BRPOS:
                case TOK_CALL:
                case TOK_DIVIDE:
                case TOK_MULT:
                case TOK_READ:
                case TOK_WRITE:
                case TOK_PUT:
                case TOK_STOP: {
                    token_t pk_t;
                    if ( i + 1 < p->n_tokens )
                        pk_t = p->tokens[i + 1];

                    if ( pk_t.token != NULL )
                    {
                        if ( strcmp( tok->token, pk_t.token ) == 0 )
                            insert( rt, pc + 1);
                    }

                    pc+=2;
                    break;
                }
                case TOK_RET:
                    pc++;
                break;
                case TOK_COPY:{

                    token_t pk_t, pk_t2;
                    if ( i + 1 < p->n_tokens )
                        pk_t = p->tokens[i + 1];

                    if ( i + 2 < p->n_tokens )
                        pk_t2 = p->tokens[i + 2];

                    if ( pk_t.token  != NULL &&
                         pk_t2.token != NULL )
                    {
                        if ( strcmp( tok->token, pk_t.token ) == 0 )
                            insert( rt, pc + 1);

                        if ( strcmp( tok->token, pk_t2.token ) == 0 )
                            insert( rt, pc + 2);
                    }

                    pc+=3;
                    break;
                }
                default:
                break;
        }

    }
    return *rt;
}

void on_save_assembled_activate( GtkMenuItem *m, GtkTextView *ct ) 
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Save File",
                                         NULL,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Save", GTK_RESPONSE_ACCEPT,
                                         NULL);

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Untitled.elf98");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(ct);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        GtkTextIter start, end;
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);

        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            g_printerr("Could not open file %s for writing\n", filename);
            g_free(text);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return;
        }

        fprintf(file, "%s", text);

        fclose(file);
        g_free(text);
        g_free(filename);
        g_print("File saved successfully\n");
    }

    gtk_widget_destroy(dialog);
}

