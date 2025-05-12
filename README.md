README.md

# STM32 TinyML Sine Wave Approximation

This project demonstrates how to train a lightweight machine learning model to approximate the sine function using TensorFlow and deploy it on an STM32 microcontroller using PlatformIO. It serves as a minimal example of how TinyML can be applied to real-world embedded systems with constrained resources.

## 🚀 Project Objective

The primary goal of this project is to:

* Train a neural network model to approximate the sine function for inputs between 0 and 2π.
* Quantize and convert the model to TensorFlow Lite (TFLite) format suitable for embedded deployment.
* Deploy and run the model on an STM32 microcontroller using PlatformIO and TensorFlow Lite for Microcontrollers.
* Demonstrate inference in real-time on the microcontroller and visualize output through UART or PWM.

## 📁 Project Structure

* development/

  * Jupyter Notebook for training and converting the model.
  * Exported TFLite model.
* deployment/

  * STM32 codebase (main.cpp, board\_setup files, etc.).
  * Model integration and inference code.

## 🛠️ Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/Mat-thias/stm32-tinyml-sine.git
cd stm32-tinyml-sine
```

### 2. Train the Model (Optional)

If you want to retrain the model:

* Open the development notebook in Google Colab or locally.
* Run all cells to train the sine model and export a .tflite file.

The resulting TFLite model will be saved and copied the deployment include directory in the C .h format.

### 3. Set Up STM32 Development Environment

Ensure you have the following installed:

* PlatformIO (can be used as VS Code extension)
* STM32CubeProgrammer (for flashing)
* Git

### 4. Configure the Board

This project targets STM32F103C8T6 ("Blue Pill"). Ensure platformio.ini is set correctly:

```ini
[env:nucleo_l476rg]
platform = ststm32
framework = stm32cube
board = nucleo_l476rg


build_flags=
    -Ilib/tflm_tree
    -Ilib/tflm_tree/tensorflow
    -Ilib/tflm_tree/tensorflow/lite
    -Ilib/tflm_tree/tensorflow/lite/micro
    
    -Ilib/tflm_tree/third_party/flatbuffers/include
    -Ilib/tflm_tree/third_party/gemmlowp
    -Ilib/tflm_tree/third_party/kissfft
    -Ilib/tflm_tree/third_party/ruy

    -DTF_LITE_STATIC_MEMORY

    -DNUCLEO_L476RG
    -std=c++17
```

### 5. Build and Upload the Code

From the root of the deployment folder:

```bash
pio run --target upload
```

Ensure your board is connected and in bootloader mode.

## ⚙️ Usage

* Once flashed, the board will read predefined input values (e.g., angle from 0 to 2π), feed them into the model, and output the sine approximation.
* The output is sent via UART or can be mapped to PWM signal for waveform reconstruction.
* Use a serial monitor (baud rate: 115200) or oscilloscope to view results.

Example serial output:

```text
sin(270°) = %-1.0000 - [15ms]
...
```

## 📦 Dependencies

* TensorFlow (for training)
* TensorFlow Lite (for model conversion)
* PlatformIO (build system)
* STM32 HAL (via Arduino/STM32duino or STM32Cube)
* Python (for development scripts)
* C++ (embedded firmware logic)
