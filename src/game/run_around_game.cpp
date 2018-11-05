#include "run_around_platform.h"

void *pushRenderCommand (render_command_list *renderCommands, 
                         render_command_type type, 
                         unsigned int size) 
{
    void *renderCommand = 0;
    int requiredSize = sizeof(render_command_header) + size;
    if (renderCommands->memory.size + requiredSize < renderCommands->memory.capacity) {
        render_command_header *header = (render_command_header *)((char *)renderCommands->memory.base + 
                                                                  renderCommands->memory.size);
        renderCommands->memory.size += requiredSize;
        header->type = type;
        renderCommand = (char *)header + sizeof(render_command_header);
    }
    else {
        // Out of memory error?
    }
    return renderCommand;
}

extern "C" void updateGame (render_command_list *renderCommands) { 
    static int counter = 0;
    counter++;
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            render_rectangle_command *rectangleCommand = 
                (render_rectangle_command *)pushRenderCommand(renderCommands, 
                                                              RENDER_COMMAND_RECTANGLE, 
                                                              sizeof(render_rectangle_command));
            rectangleCommand->x = i * 100 + counter;
            rectangleCommand->y = i * 100 + counter;
            rectangleCommand->width = 100;
            rectangleCommand->height = 100;
            rectangleCommand->color = 0xffffff00;
        }
        else {
            render_horizontal_line_command *lineCommand = 
                (render_horizontal_line_command *)pushRenderCommand(renderCommands, 
                                                          RENDER_COMMAND_HORIZONTAL_LINE, 
                                                          sizeof(render_horizontal_line_command));
            lineCommand->lineNum = (char)((i+counter) * 2);
            lineCommand->color = 0xff00ffff;
        }
    }
}
