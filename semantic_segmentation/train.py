from model.Enet import *
from data_processing import *
from keras.callbacks import TensorBoard
from PIL import ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True
os.environ["CUDA_VISIBLE_DEVICES"] = "0"

data_gen_args = dict()
tensorboard = TensorBoard(log_dir='./logs_road_dicebce')

car = trainGenerator(4,'/home/himanshu/dl/dataset/cityscape', 'images', 'masks', data_gen_args, image_color_mode = "rgb", target_size = (512,512), save_to_dir = None)
model = Enet()

model_checkpoint = ModelCheckpoint('/home/himanshu/dl/adb/road_dicebce.hdf5', monitor = 'loss', verbose=1, save_best_only=True)
model.fit_generator(car, steps_per_epoch = 6000, epochs = 200, callbacks = [tensorboard, model_checkpoint])
