#pragma once
// Storage<T> - A fixed-capacity dynamic array template (no std::vector allowed)
// Header-only template class

template<typename T>
class Storage {
    private:
        T** data;
        int count;
        int capacity;

        void resize() {
            int newCapacity = capacity * 2;
            T** newData = new T*[newCapacity];
            for(int i = 0; i < count; i++) newData[i] = data[i];
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

    public:
        Storage(int initialCapacity = 64) : count(0), capacity(initialCapacity) {
            data = new T*[capacity];
        }

        ~Storage() {
            clear();
            delete[] data;
        }

        // No copy (prevent double free)
        Storage(const Storage&) = delete;
        Storage& operator=(const Storage&) = delete;

        void add(T* item) {
            if(count >= capacity) resize();
            data[count++] = item;
        }

        T* get(int index) const {
            if(index < 0 || index >= count) return nullptr;
            return data[index];
        }

        int getCount() const { return count; }

        // Remove by index (does NOT delete the object)
        void removeAt(int index) {
            if(index < 0 || index >= count) return;
            for(int i = index; i < count - 1; i++) data[i] = data[i+1];
            count--;
        }

        // Remove and delete the object at index
        void deleteAt(int index) {
            if(index < 0 || index >= count) return;
            delete data[index];
            for(int i = index; i < count - 1; i++) data[i] = data[i+1];
            count--;
        }

        // Clear and delete all objects
        void clear() {
            for(int i = 0; i < count; i++) { delete data[i]; data[i] = nullptr; }
            count = 0;
        }

        // Find by ID (T must have getId())
        T* findById(int id) const {
            for(int i = 0; i < count; i++) {
                if(data[i]->getId() == id) return data[i];
            }
            return nullptr;
        }

        // Remove by ID and delete object
        bool deleteById(int id) {
            for(int i = 0; i < count; i++) {
                if(data[i]->getId() == id) {
                    deleteAt(i);
                    return true;
                }
            }
            return false;
        }

        // Bubble sort ascending by date (for Appointment/Prescription which have getDate())
        void sortByDateAsc() {
            for(int i = 0; i < count - 1; i++) {
                for(int j = 0; j < count - i - 1; j++) {
                    if(compareDates(data[j]->getDate(), data[j+1]->getDate()) > 0) {
                        T* tmp = data[j]; data[j] = data[j+1]; data[j+1] = tmp;
                    }
                }
            }
        }

        void sortByDateDesc() {
            for(int i = 0; i < count - 1; i++) {
                for(int j = 0; j < count - i - 1; j++) {
                    if(compareDates(data[j]->getDate(), data[j+1]->getDate()) < 0) {
                        T* tmp = data[j]; data[j] = data[j+1]; data[j+1] = tmp;
                    }
                }
            }
        }

        // Sort by time slot ascending (for Appointment which has getTimeSlot())
        void sortByTimeAsc() {
            for(int i = 0; i < count - 1; i++) {
                for(int j = 0; j < count - i - 1; j++) {
                    if(strEqual(data[j]->getTimeSlot(), data[j+1]->getTimeSlot()) == false) {
                        // Compare HH:MM lexicographically
                        bool swap = false;
                        const char* t1 = data[j]->getTimeSlot();
                        const char* t2 = data[j+1]->getTimeSlot();
                        for(int k = 0; t1[k] != '\0' && t2[k] != '\0'; k++) {
                            if(t1[k] > t2[k]) { swap = true; break; }
                            if(t1[k] < t2[k]) break;
                        }
                        if(swap) { T* tmp = data[j]; data[j] = data[j+1]; data[j+1] = tmp; }
                    }
                }
            }
        }
};
