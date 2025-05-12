# STM32 TinyML Sine Wave Approximation

This project demonstrates how to train a lightweight machine learning model to approximate the sine function using TensorFlow and deploy it on an STM32 microcontroller using PlatformIO to control the onboard LED in a sinusoidal pattern. It serves as a minimal example of how TinyML can be applied to real-world embedded systems with constrained resources.

## 🚀 Project Objective

The primary goals of this project are to:

* Train a neural network model to approximate the sine function for inputs between 0 and 2π.
* Quantize and convert the model to TensorFlow Lite (TFLite) format suitable for embedded deployment.
* Deploy and run the model on an STM32 microcontroller using PlatformIO and TensorFlow Lite for Microcontrollers.
* Demonstrate inference in real-time on the microcontroller and visualize output through UART and PWM. The output is printed via serial and used to vary the brightness of the onboard LED in a sinusoidal pattern.

## 📁 Project Structure

* development/

  * Jupyter notebook for training and converting the model.
  * requirements.txt for Python dependencies.
  * Exported TFLite model in .tflite format.
* deployment/

  * STM32 codebase (main.cpp, board\_setup files, etc.).
  * Model integration and inference logic.
  * Include directory for compiled model header file.

## 🛠️ Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/Mat-thias/stm32-tinyml-sine.git
cd stm32-tinyml-sine
```

### 2. Set Up Python Environment for Model Training

Install required Python packages:

```bash
cd development
pip install -r requirements.txt
```

You can run this on your local machine or in a cloud notebook like Google Colab.

### 3. Train and Export the Model (Optional)

To retrain the model:

* Open the Jupyter notebook in the development directory.
* Run all cells to train the sine model and export a .tflite file.
* The resulting TFLite model will be converted to a C array and saved in the deployment include directory.

### 4. Set Up STM32 Development Environment

Ensure the following tools are installed:

* PlatformIO (CLI or VS Code extension)
* STM32CubeProgrammer (for flashing)
* Git

### 5. Configure the Board

This project targets the STM32 Nucleo-L476RG. Ensure platformio.ini is configured as follows:

```ini
[env:nucleo_l476rg]
platform = ststm32
framework = stm32cube
board = nucleo_l476rg

build_flags =
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

Modify the board configuration if you're using a different STM32 target.

### 6. Build and Upload the Code

Navigate to the deployment folder and run:

```bash
pio run --target upload
```

Make sure your board is connected and in bootloader mode.

## ⚙️ Usage

* After flashing, the board will simulate input angles (0 to 2π), feed them into the trained model, and produce sine values.
* Output is sent over UART and used to control the onboard LED using PWM for visual effect.
* Use a serial monitor (baud rate: 115200) or an oscilloscope to observe output.

Example serial output:

```text
sin(270°) = %-1.0000 - [15ms]
...
```

## 📦 Dependencies

Development (Python):

* TensorFlow
* NumPy
* matplotlib

See requirements.txt in the development/ folder.

Deployment (STM32):

* PlatformIO
* STM32Cube HAL
* TensorFlow Lite for Microcontrollers
* C++17
