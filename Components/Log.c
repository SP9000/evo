/*****************************************************************************/
/* Log.c                                                                     */
/* This component is a widget used to display data from various files.       */
/* Bryce Wilson                                                              */
/* Created: July 22, 2013                                                    */
/*****************************************************************************/

#if 0
COMPONENT Log : Widget {
    public char* log_file;

    GString str;
    int last_modified;

    FILE* file;
    int file_d;
    TextRenderer* text_renderer;

    public void SetLogFile(int fd)
    {
        self->file_d = freopen(log_file, "r", fd);
        self->file = fdopen(file_d);
        self->text_renderer = Component_GetAs(TextRenderer);
    }

    void Start() 
    {
        self->str = g_string_new(NULL);
    }

    void Update()
    {
        struct stat s;
        /* stat the file and check for errors */
        int err = fstat(self->file_d);
        if(err != 0) {
            return;
        }
        /* if the modified time is new, read the new data into the string */
        if(last_modified < s.st_mtime)  {
            while(!feof(self->fp)) {
                g_string_append_c(str, fgetc(self->fp));
            }
        }
    }
    void Collide(Entity* other)
    {

    }
}

#endif
