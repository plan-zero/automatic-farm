# You should have received a copy of the GNU General Public License
# along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(SYSTEM-TIMER)

set(HEADER_INC
    ./inc
    ./../../drivers/timer/inc
    ./../../hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./src/system-timer.c
)
avr_add_library(system-timer ${SOURCE_FILES})
target_link_libraries(system-timer timer)