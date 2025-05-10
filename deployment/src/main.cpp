#include "board_setup.h"


#define DO_FOREVER          while(1) // I like this than loop or while(1) for an infinite loop
#define ONE_CYCLE           360 // 1 cycle is 360
#define PI                  2.f * 3.14159265359f

#define TENSOR_ARENA_SIZE   2 * 1024

// The necessary tensorflowlite-micro library 
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

// To get the model 
#include "sine_model.h"

int main(void) {

    HAL_Init();
    init_GPIO_pins();
    init_UART2();
    init_TIM2();

    const tflite::Model* model = tflite::GetModel(sine_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        // print("The model version of %d does not match the version of the schema of version %d", model->version(), TFLITE_SCHEMA_VERSION);
    }
    
    tflite::MicroMutableOpResolver<2> resolver;
    if (resolver.AddFullyConnected() != kTfLiteOk || resolver.AddRelu() != kTfLiteOk) {
        // print("Failed to add all the ops.");
        return -1;
    }


    // Keep aligned to 16 bytes for CMSIS
    alignas(16) uint8_t tensor_arena[TENSOR_ARENA_SIZE];

    tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, TENSOR_ARENA_SIZE);
    tflite::MicroInterpreter* interpreter = &static_interpreter;
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        // print("Failed to allocate tensors.\n");
        return -1;
    }

    TfLiteTensor* input = interpreter->input(0);
    TfLiteTensor* output = interpreter->output(0);

    DO_FOREVER{
        for(int i=0; i<ONE_CYCLE; i++) {
            uint32_t start_ms = HAL_GetTick();

            float x = (float) i / ONE_CYCLE * PI;
            input->data.f[0] = x;

            if (interpreter->Invoke() != kTfLiteOk) {
                // print("Failed to invoke for sin(%d) deg", x);
                continue;
            }
            float y = input->data.f[0];

            analogWrite((uint8_t) ((y + 1) * (100 / 2)));
            // sprintf(buffer, "sin(%3d°) = %7.4f - [%ums]\n", i, y, (uint32_t)HAL_GetTick() - start_ms);
            // print(buffer);
        }
    }
}