#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <list>
#define total_words 18

class CommandProcessor
{
private:
    QueueHandle_t m_command_queue_handle;
    void processCommand(uint16_t commandIndex);

public:
    CommandProcessor();
    void queueCommand(uint16_t commandIndex, float best_score);
    friend void commandQueueProcessorTask(void *param);
};

#endif
