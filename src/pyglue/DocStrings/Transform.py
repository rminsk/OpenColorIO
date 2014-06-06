
class Transform:
    """
    These are typically only needed when creating or manipulating configurations.
    """
    def __init__(self):
        pass
        
    def isEditable(self):
        pass
        
    def createEditableCopy(self):
        pass
        
    def getDirection(self):
        pass
        
    def setDirection(self):
        pass

    def serialize(self):
        """
        serialize()
        
        Returns the string representation of :py:class:`PyOpenColorIO.Transform`
        in YAML text form. This is typically stored on disk in a file with the
        .ociot extension.
        
        :return: :py:class:`PyOpenColorIO.Transform` in YAML text form
        :rtype: string
        """
        pass

