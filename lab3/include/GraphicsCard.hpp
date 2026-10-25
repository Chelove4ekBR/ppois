#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ByteSize.hpp"
#include "Display.hpp"
#include "Gpu.hpp"
#include "VideoPort.hpp"

/**
 * @brief Представляет дискретную видеокарту, включающую GPU, видеопамять VRAM и видеовыходы.
 */
class GraphicsCard {
public:
    /**
     * @brief Создаёт видеокарту.
     * @param gpu Графический процессор.
     * @param vramSize Объём выделенной видеопамяти VRAM.
     * @param pcieLanesNeeded Число линий PCIe (обычно 16).
     */
    GraphicsCard(Gpu gpu, ByteSize vramSize, uint32_t pcieLanesNeeded = 16);

    /**
     * @brief Возвращает GPU видеокарты.
     * @return Ссылка на Gpu.
     */
    Gpu& gpu() { return gpu_; }

    /**
     * @brief Возвращает GPU видеокарты.
     * @return Константная ссылка на Gpu.
     */
    const Gpu& gpu() const { return gpu_; }

    /**
     * @brief Возвращает объём видеопамяти.
     * @return Объём ByteSize.
     */
    const ByteSize& vramSize() const { return vramSize_; }

    /**
     * @brief Возвращает список видеопортов.
     * @return Вектор видеопортов.
     */
    const std::vector<VideoPort>& ports() const { return ports_; }

    /**
     * @brief Возвращает требуемое число линий PCIe.
     * @return Число линий шины.
     */
    uint32_t pcieLanesNeeded() const { return pcieLanesNeeded_; }

    /**
     * @brief Возвращает список подключённых дисплеев.
     * @return Вектор указателей на Display.
     */
    const std::vector<DisplayPtr>& connectedDisplays() const { return connectedDisplays_; }

    /**
     * @brief Добавляет видеопорт на панель видеокарты.
     * @param port Видеопорт.
     */
    void addPort(const VideoPort& port);

    /**
     * @brief Подключает монитор к указанному видеопорту.
     * @param portId Номер порта.
     * @param display Умный указатель на Display.
     */
    void connectDisplay(uint32_t portId, DisplayPtr display);

    /**
     * @brief Отключает монитор от видеопорта.
     * @param portId Номер порта.
     */
    void disconnectDisplay(uint32_t portId);

    /**
     * @brief Отрисовывает 3D-сцену и передаёт кадр на все подключённые мониторы.
     * @param polygonCount Число полигонов в кадре.
     * @param frameId Порядковый номер кадра.
     * @return Время отрисовки кадра в миллисекундах.
     */
    double render3DScene(uint32_t polygonCount, uint64_t frameId);

    /**
     * @brief Рассчитывает количество фактически подключённых мониторов.
     * @return Число мониторов.
     */
    uint32_t connectedDisplaysCount() const;

    /**
     * @brief Возвращает общее количество видеопортов.
     * @return Количество портов.
     */
    uint32_t totalPortsCount() const;

    /**
     * @brief Охлаждает видеокарту.
     * @param deltaC Снижение температуры в °C.
     */
    void coolDown(double deltaC);

private:
    Gpu gpu_;
    ByteSize vramSize_;
    std::vector<VideoPort> ports_;
    uint32_t pcieLanesNeeded_;
    std::vector<DisplayPtr> connectedDisplays_;
};

using GraphicsCardPtr = std::shared_ptr<GraphicsCard>;
