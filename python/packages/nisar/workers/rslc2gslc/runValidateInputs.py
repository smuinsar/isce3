# -*- coding: utf-8 -*-

'''
This is the first step of the workflow
'''


def runValidateInputs(self):
    '''
    This steps loads the yaml file and sets up initial state
     of workflow.
    '''

    ##This section is for logic checks
    ##Update state if needed / flag errors in inputs

    state = self.state
    state.input_hdf5 = self.get_value(['inputs', 'rslc'])
    if not state.input_hdf5:
        raise AttributeError('ERROR the following argument is required:'
                             'inputs.rslc')
    state.output_hdf5 = self.get_value(['outputs', 'gslc'])

    # nlooks
    #nlooks_az = self.get_value(['parameters', 'pre_process', 'azimuth_looks'])
    #nlooks_rg = self.get_value(['parameters', 'pre_process', 'range_looks'])
    #state.nlooks_az = self.cast_input(nlooks_az, dtype=int, 
    #    default=1)
    #state.nlooks_rg = self.cast_input(nlooks_rg, dtype=int, 
    #    default=1)

# end of file
