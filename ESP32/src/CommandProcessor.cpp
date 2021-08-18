#include <Arduino.h>
#include "CommandProcessor.h"
#include "Display.h"
#include <esp_task_wdt.h>

float display_percentage=0;
const char *words[] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "tree",
    "bird",
    "cat",
    "dog",
    "happy",
    "house",
    "wow",
    "invalid",
};

void commandQueueProcessorTask(void *param)
{
    CommandProcessor *commandProcessor = (CommandProcessor *)param;
    while (true)
    {
        uint16_t commandIndex = 0;
        if (xQueueReceive(commandProcessor->m_command_queue_handle, &commandIndex, portMAX_DELAY) == pdTRUE)
        {
            commandProcessor->processCommand(commandIndex);
        }
    }
}

int calcDuty(int ms)
{
    // 50Hz = 20ms period
    return (65536 * ms) / 20000;
}

void CommandProcessor::processCommand(uint16_t commandIndex)
{
    digitalWrite(GPIO_NUM_2, HIGH);
    print_in_display(words[commandIndex],display_percentage);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(GPIO_NUM_2, LOW);
    display_percentage = 0;
}

CommandProcessor::CommandProcessor()
{
    pinMode(GPIO_NUM_2, OUTPUT);

    // allow up to 5 commands to be in flight at once
    m_command_queue_handle = xQueueCreate(total_words, sizeof(uint16_t));
    if (!m_command_queue_handle)
    {
        Serial.println("Failed to create command queue");
    }
    // kick off the command processor task
    TaskHandle_t command_queue_task_handle;
    xTaskCreate(commandQueueProcessorTask, "Command Queue Processor", 2048, this, 1, &command_queue_task_handle);
    esp_task_wdt_add(command_queue_task_handle);
    esp_task_wdt_init(10, false);
}

void CommandProcessor::queueCommand(uint16_t commandIndex, float best_score)
{
    // unsigned long now = millis();
    if (commandIndex != total_words && commandIndex != -1)
    {
        Serial.printf("***** %ld Detected command %s(%f)\n", millis(), words[commandIndex], best_score);
        display_percentage = best_score;
        if (xQueueSendToBack(m_command_queue_handle, &commandIndex, 0) != pdTRUE)
        {
            Serial.println("No more space for command");
        }
    }
}
