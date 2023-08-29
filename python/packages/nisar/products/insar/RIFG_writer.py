import h5py
import numpy as np
from nisar.workflows.h5_prep import get_off_params
from nisar.workflows.helpers import get_cfg_freq_pols

from .common import InSARProductsInfo
from .dataset_params import DatasetParams, add_dataset_and_attrs
from .InSAR_L1_writer import L1InSARWriter
from .product_paths import RIFGGroupsPaths


class RIFGWriter(L1InSARWriter):
    """
    Writer class for RIFG product inherenting from L1InSARWriter
    """

    def __init__(self, **kwds):
        """
        Constructor for RIFG class
        """
        super().__init__(**kwds)

        # RIFG group paths
        self.group_paths = RIFGGroupsPaths()

        # RIFG product information
        self.product_info = InSARProductsInfo.RIFG()

    def add_root_attrs(self):
        """
        add root attributes
        """

        super().add_root_attrs()

        # Add additional attributes
        self.attrs["title"] = np.string_("NISAR L1_RIFG Product")
        self.attrs["reference_document"] = np.string_("JPL-102270")

        ctype = h5py.h5t.py_create(np.complex64)
        ctype.commit(self["/"].id, np.string_("complex64"))
        
    def add_algorithms_to_procinfo(self):
        """
        Add the algorithms to processingInformation group

        Returns
        ----------
        algo_group : h5py.Group)
            the algorithm group object
        """
        
        algo_group = super().add_algorithms_to_procinfo()
        self.add_interferogramformation_to_algo(algo_group)
        
        return algo_group
        
    def add_interferogram_to_swaths(self, rg_looks: int, az_looks: int):
        """
        Add interferogram group to swaths
        """
        
        super().add_interferogram_to_swaths(rg_looks, az_looks)
        
        # Add the wrappedInterferogram to the interferogram group
        # under swaths group
        for freq, pol_list, _ in get_cfg_freq_pols(self.cfg):
            swaths_freq_group_name = (
                f"{self.group_paths.SwathsPath}/frequency{freq}"
            )
            slc_dset = self.ref_h5py_file_obj[
                f'{f"{self.ref_rslc.SwathPath}/frequency{freq}"}/{pol_list[0]}'
            ]
            slc_lines, slc_cols = slc_dset.shape

            # shape of the interferogram product
            igram_shape = (slc_lines // az_looks,slc_cols // rg_looks)

            # add the wrappedInterferogram to each polarization group
            for pol in pol_list:
                # create the interferogram dataset
                igram_pol_group_name = \
                    f"{swaths_freq_group_name}/interferogram/{pol}"
                igram_pol_group = self.require_group(igram_pol_group_name)

                # The interferogram dataset parameters including the 
                # dataset name, dataset data type, description, units
                igram_ds_params = [
                    (
                        "wrappedInterferogram",
                        np.complex64,
                        f"Interferogram between {pol} layers",
                        "DN",
                    ),
                ]
                
                for igram_ds_param in igram_ds_params:
                    ds_name, ds_dtype, ds_description, ds_unit \
                        = igram_ds_param
                    self._create_2d_dataset(
                        igram_pol_group,
                        ds_name,
                        igram_shape,
                        ds_dtype,
                        ds_description,
                        units=ds_unit,
                    )
                  
    def add_swaths_to_hdf5(self):
        """
        Add swaths to the HDF5
        """
        
        super().add_swaths_to_hdf5()
        
        # add the inteferogram group
        proc_cfg = self.cfg["processing"]
        rg_looks = proc_cfg["crossmul"]["range_looks"]
        az_looks = proc_cfg["crossmul"]["azimuth_looks"]
        
        # add subswaths to swaths group
        self.add_subswaths_to_swaths(rg_looks)
        self.add_interferogram_to_swaths(rg_looks, az_looks)
