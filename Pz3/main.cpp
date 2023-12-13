from pydbus import SystemBus
from gi.repository import GLib
import time
import logging
from systemd.journal import JournalHandler
from systemdlogging.toolbox import init_systemd_logging # it attaches systemd logging handler to a root Python logger.

def Connection_state(state):
 if state:
  global start
  start = time.perf_counter()
  logger.debug('You've connected')
 else:
  global end
  end = time.perf_counter()
  time_spent = end - start
  logger.debug ('No connection!')
  logger.debug('Total time: ', time_spent)

def Connection_change(state):
 logger.debug(state)
 if state == 70: # NM_DEVICE_STATE_IP_CONFIG
  Connection_state(TRUE)
 else:
  if state == 20: # NM_DEVICE_STATE_UNAVAILABLE
   Connection_state(FALSE)

init_systemd_logging() # Returns True if initialization went fine.
logger = logging.getLogger('my_logger')
logger.setLevel(logging.DEBUG)
bus = pydbus.SystemBus()
proxy = bus.get('org.freedesktop.NetworkManager', '/org/freedesktop/NetworkManager/Devices/0')
Connection_change(proxy.state())
proxy.onStateChanged = Connection_change

loop = GLib.MainLoop()
loop.run()
