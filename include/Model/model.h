#ifndef _MODEL_H_
#define _MODEL_H_

#include "../define.h"
#include "model_part.h"

#include <string>
#include <unordered_map>
#include <memory>

class Model
{
    public:
        /// @name Type Define
        /// @{
            typedef ModelPart::IndexType
                                                                                IndexType;
            LOTUS_SHARED_POINTER_DEFINE(Model)

        /// @}


        /// @name Life Circle
        /// @{
            /// Default constructor.
            Model(){};

            /// Destructor.
            ~Model()
            {
                mRootModelPartMap.clear();
            }

            Model(const Model&) = delete;


        /// @}


        /// @name Operators
        /// @{
            Model & operator=(const Model&) = delete;

        /// @}


        /// @name Operations
        /// @{
              /**
             * @brief This method clears the database of modelparts
             * @details Executes a clear on the model part map
             */
            void Reset();

            /**
             * @brief This method creates a new model part contained in the current Model with a given name and buffer size
             * @param ModelPartName The name of the new model part to be created
             * @param NewBufferSize The size of the buffer of the new model part created
             */
            ModelPart& CreateModelPart( const std::string& ModelPartName, IndexType NewBufferSize=1 );

            /**
             * @brief This method deletes a modelpart with a given name
             * @details Raises a warning in case the model part does not exists
             * @param ModelPartName The name of the model part to be removed
             */
            void DeleteModelPart( const std::string& ModelPartName );

            /**
             * @brief This method renames a modelpart with a given name
             * @details Raises an error in case the model part does not exists as root model part
             * @param OldName The name of the model part to be renamed
             * @param NewName The new name for the model part to be renamed
             */
            void RenameModelPart( const std::string& OldName, const std::string& NewName );


        /// @}


        /// @name Access
        /// @{
            /**
             * @brief This method returns a model part given a certain name
             * @details Iterates over the list of submodelparts of the root model part
             * @param rFullModelPartName The name of the model part to be returned
             * @return Reference to the model part of interest
             */
            ModelPart& GetModelPart(const std::string& rFullModelPartName);

            /**
             * @brief This method returns a model part given a certain name
             * @details Iterates over the list of submodelparts of the root model part
             * @param rFullModelPartName The name of the model part to be returned
             * @return Reference to the model part of interest
             */
            const ModelPart& GetModelPart(const std::string& rFullModelPartName) const;

             /**
             * @brief This returns a vector containing a list of model parts names contained on the model
             * @details Iterates over the list of submodelparts of the root model part
             * @return A vector of strings containing the model parts names
             */
            std::vector<std::string> GetModelPartNames() const;
        /// @}


        /// @name Inquiry
        /// @{
            /**
             * @brief This method checks if a certain a model part exists given a certain name
             * @details Iterates over the list of submodelparts of the root model part
             * @param rFullModelPartName The name of the model part to be checked
             * @return True if the model part exists, false otherwise
             */
            bool HasModelPart(const std::string& rFullModelPartName) const;

        /// @}

        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            std::string Info() const;

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const;

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const;
        /// @}
    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            

        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{


        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
        /// @name Private Static Member Variables
        /// @{


        /// @}


        /// @name Private Member Variables
        /// @{
            std::map< std::string, std::unique_ptr<ModelPart> > mRootModelPartMap; /// The map containing the list of model parts

        /// @}


        /// @name Private Operatiors
        /// @{
            /**
             * @brief This method searchs recursively a sub model part in a model part
             * @param rModelPartName The name to be search
             * @param pModelPart Pointer of the model part where search recursively
             * @return The pointer of the model part of interest
             */
            ModelPart* RecursiveSearchByName(const std::string& rModelPartName, ModelPart* pModelPart) const;

            /**
             * @brief This method splits the name of the model part using "." to define the hierarchy
             * @param rFullModelPartName The name with the full hierarchy
             * @return The vector containing each part of the name defining the model part hierarchy
             */
            std::vector<std::string> SplitSubModelPartHierarchy(const std::string& rFullModelPartName) const;

            /**
             * @brief This method creates a new model part contained in the current Model with a given name and buffer size
             * @param ModelPartName The name of the new model part to be created
             * @param NewBufferSize The size of the buffer of the new model part created
             */
            void CreateRootModelPart(const std::string& ModelPartName, ModelPart::IndexType NewBufferSize);


        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif
